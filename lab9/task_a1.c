#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main() {
    int p1[2], p2[2]; // Two pipes: p1 (Parent->Child), p2 (Child->Parent)
    
    if (pipe(p1) == -1 || pipe(p2) == -1) {
        perror("pipe");
        return 1;
    }

    if (fork() == 0) {          // Child
        close(p1[1]); // Close write end of parent-to-child
        close(p2[0]); // Close read end of child-to-parent

        char buf[100];
        read(p1[0], buf, sizeof(buf));
        printf("Child read: %s\n", buf);
        
        char reply[] = "Hello from child!";
        write(p2[1], reply, strlen(reply) + 1);
        
        close(p1[0]);
        close(p2[1]);
    } else {                    // Parent
        close(p1[0]); // Close read end of parent-to-child
        close(p2[1]); // Close write end of child-to-parent

        char msg[] = "Hello from parent";
        write(p1[1], msg, strlen(msg) + 1);
        
        char buf[100];
        read(p2[0], buf, sizeof(buf));
        printf("Parent received reply: %s\n", buf);
        
        close(p1[1]);
        close(p2[0]);
        wait(NULL);
    }
    return 0;
}
