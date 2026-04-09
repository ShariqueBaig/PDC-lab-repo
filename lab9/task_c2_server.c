#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>

#define SOCKET_PATH "/tmp/mysocket"

int main() {
    int server_fd, client_fd;
    struct sockaddr_un addr;
    char buf[1024];

    // Create socket
    if ((server_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    // Set address structure
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    // Unlink old socket file
    unlink(SOCKET_PATH);

    // Bind
    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)) == -1) {
        perror("bind");
        exit(1);
    }

    // Listen
    if (listen(server_fd, 5) == -1) {
        perror("listen");
        exit(1);
    }

    printf("Server listening on %s...\n", SOCKET_PATH);

    // Accept connection
    if ((client_fd = accept(server_fd, NULL, NULL)) == -1) {
        perror("accept");
        exit(1);
    }

    // Read message
    read(client_fd, buf, sizeof(buf));
    printf("Server received: %s\n", buf);

    // Echo back
    strcat(buf, " - Server Echo");
    write(client_fd, buf, strlen(buf) + 1);

    close(client_fd);
    close(server_fd);
    unlink(SOCKET_PATH);

    return 0;
}
