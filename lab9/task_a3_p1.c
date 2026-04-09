#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

int main() {
    char buf[100];
    
    // Create FIFOs
    mkfifo("fifo1", 0666);
    mkfifo("fifo2", 0666);

    printf("Process 1: Opening fifo1 for writing...\n");
    int fd_write = open("fifo1", O_WRONLY);
    
    printf("Process 1: Opening fifo2 for reading...\n");
    int fd_read = open("fifo2", O_RDONLY);

    // Write to P2
    char msg[] = "Hello from Process 1 via fifo1";
    write(fd_write, msg, strlen(msg) + 1);
    printf("Process 1: Sent message.\n");

    // Read from P2
    read(fd_read, buf, sizeof(buf));
    printf("Process 1: Received reply: %s\n", buf);

    close(fd_write);
    close(fd_read);
    
    return 0;
}
