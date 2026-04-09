#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main() {
    int sockets[2];
    char buf[1024];

    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sockets) < 0) {
        perror("socketpair");
        exit(1);
    }

    if (fork() == 0) {
        // Child
        close(sockets[0]); // Use sockets[1]
        
        // Read parent's message
        read(sockets[1], buf, sizeof(buf));
        printf("Child received: %s\n", buf);
        
        // Echo back
        strcat(buf, " (Echo from child)");
        write(sockets[1], buf, strlen(buf) + 1);
        
        close(sockets[1]);
        exit(0);
    } else {
        // Parent
        close(sockets[1]); // Use sockets[0]
        
        // Send message
        char *msg = "Hello child!";
        write(sockets[0], msg, strlen(msg) + 1);
        
        // Read echo
        read(sockets[0], buf, sizeof(buf));
        printf("Parent received echo: %s\n", buf);
        
        close(sockets[0]);
        wait(NULL);
    }

    return 0;
}
