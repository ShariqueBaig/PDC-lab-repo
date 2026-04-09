#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>

#define SERVER_PATH "/tmp/udp_server"

int main() {
    int server_fd;
    struct sockaddr_un server_addr, client_addr;
    socklen_t client_len;
    char buf[1024];

    // Create socket
    if ((server_fd = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    // Set address structure
    memset(&server_addr, 0, sizeof(struct sockaddr_un));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SERVER_PATH, sizeof(server_addr.sun_path) - 1);

    unlink(SERVER_PATH);

    // Bind
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_un)) == -1) {
        perror("bind");
        exit(1);
    }

    printf("UDP Server listening on %s...\n", SERVER_PATH);

    while (1) {
        client_len = sizeof(struct sockaddr_un);
        ssize_t n = recvfrom(server_fd, buf, sizeof(buf), 0, (struct sockaddr *)&client_addr, &client_len);
        if (n < 0) {
            perror("recvfrom");
            continue;
        }

        buf[n] = '\0';
        printf("Received from %s: %s\n", client_addr.sun_path, buf);

        // Echo back
        strcat(buf, " - UDP Echo");
        if (sendto(server_fd, buf, strlen(buf), 0, (struct sockaddr *)&client_addr, client_len) < 0) {
            perror("sendto");
        }
    }

    close(server_fd);
    unlink(SERVER_PATH);
    return 0;
}
