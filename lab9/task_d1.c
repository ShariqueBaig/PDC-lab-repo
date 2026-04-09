#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

struct msgbuf {
    long mtype;
    char mtext[256];
};

int main() {
    system("touch mqfile");
    key_t key  = ftok("mqfile", 42);
    int  msqid = msgget(key, 0666 | IPC_CREAT);

    struct msgbuf msg;

    if (fork() == 0) {          // Child
        // Receive from parent
        msgrcv(msqid, &msg, sizeof(msg.mtext), 1, 0);
        printf("Child received: %s\n", msg.mtext);
        
        // Reply to parent
        msg.mtype = 2;
        snprintf(msg.mtext, sizeof(msg.mtext), "Hello from child via MQ");
        msgsnd(msqid, &msg, sizeof(msg.mtext), 0);
        
        exit(0);
    } else {                    // Parent
        // Send to child
        msg.mtype = 1;
        snprintf(msg.mtext, sizeof(msg.mtext), "Hello from parent via MQ");
        msgsnd(msqid, &msg, sizeof(msg.mtext), 0);
        printf("Parent sent message.\n");

        wait(NULL);

        // Receive from child
        msgrcv(msqid, &msg, sizeof(msg.mtext), 2, 0);
        printf("Parent received reply: %s\n", msg.mtext);

        msgctl(msqid, IPC_RMID, NULL);
    }
    return 0;
}
