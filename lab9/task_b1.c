#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    // Ensure shmfile exists
    system("touch shmfile");

    key_t key  = ftok("shmfile", 65);
    int   shmid = shmget(key, 1024, 0666 | IPC_CREAT);
    char *str  = (char*) shmat(shmid, NULL, 0);

    if (fork() == 0) {          // Child
        sleep(2);               // Wait for parent to write
        printf("Child read from memory: %s\n", str);
        
        strcat(str, " - ACK");  // Modify message
        printf("Child appended ACK.\n");
        
        shmdt(str);
        exit(0);
    } else {                    // Parent
        strcpy(str, "Shared memory message");
        printf("Parent wrote: %s\n", str);
        
        shmdt(str);             // Detach to wait for child
        
        wait(NULL);             // Wait for child to finish modification
        
        // Re-attach to read result
        str = (char*) shmat(shmid, NULL, 0);
        printf("Parent read modified memory: %s\n", str);
        
        shmdt(str);
        shmctl(shmid, IPC_RMID, NULL);
    }
    return 0;
}
