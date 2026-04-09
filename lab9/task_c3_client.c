#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>

#define SERVER_PATH "/tmp/udp_server"

int main() {
    int client_fd;
    struct sockaddr_un server_addr, client_addr;
    char buf[1024];
    char client_path[100];

    // Create unique client path based on PID
    snprintf(client_path, sizeof(client_path), "/tmp/udp_client_%d", getpid());

    if ((client_fd = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    // Bind client socket (needed for receiving replies in AF_UNIX DGRAM)
    memset(&client_addr, 0, sizeof(struct sockaddr_un));
    client_addr.sun_family = AF_UNIX;
    strncpy(client_addr.sun_path, client_path, sizeof(client_addr.sun_path) - 1);
    unlink(client_path);

    if (bind(client_fd, (struct sockaddr *)&client_addr, sizeof(struct sockaddr_un)) == -1) {
        perror("bind");
        exit(1);
    }

    // Set server address
    memset(&server_addr, 0, sizeof(struct sockaddr_un));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SERVER_PATH, sizeof(server_addr.sun_path) - 1);

    // Send message
    char *msg = "Hello from UDP client!";
    if (sendto(client_fd, msg, strlen(msg), 0, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_un)) < 0) {
        perror("sendto");
        exit(1);
    }

    // Receive echo
    ssize_t n = recvfrom(client_fd, buf, sizeof(buf), 0, NULL, NULL);
    if (n < 0) {
        perror("recvfrom");
    } else {
        buf[n] = '\0';
        printf("Client received echo: %s\n", buf);
    }

    close(client_fd);
    unlink(client_path);
    return 0;
}
