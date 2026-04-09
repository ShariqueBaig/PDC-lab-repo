#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

int main() {
    char buf[100];
    
    // Create FIFOs (in case P1 didn't)
    mkfifo("fifo1", 0666);
    mkfifo("fifo2", 0666);

    printf("Process 2: Opening fifo1 for reading...\n");
    int fd_read = open("fifo1", O_RDONLY);

    printf("Process 2: Opening fifo2 for writing...\n");
    int fd_write = open("fifo2", O_WRONLY);

    // Read from P1
    read(fd_read, buf, sizeof(buf));
    printf("Process 2: Received message: %s\n", buf);

    // Reply to P1
    char reply[] = "Hello from Process 2 via fifo2";
    write(fd_write, reply, strlen(reply) + 1);
    printf("Process 2: Sent reply.\n");

    close(fd_read);
    close(fd_write);

    return 0;
}
