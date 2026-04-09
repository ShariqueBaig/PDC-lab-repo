#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>

#define SOCKET_PATH "/tmp/mysocket"

int main() {
    int client_fd;
    struct sockaddr_un addr;
    char buf[1024];

    // Create socket
    if ((client_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    // Set address structure
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    // Connect
    if (connect(client_fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)) == -1) {
        perror("connect");
        exit(1);
    }

    // Send message
    char *msg = "Hello from client!";
    write(client_fd, msg, strlen(msg) + 1);

    // Read echo
    read(client_fd, buf, sizeof(buf));
    printf("Client received echo: %s\n", buf);

    close(client_fd);

    return 0;
}
