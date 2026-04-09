#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

struct msgbuf {
    long mtype;
    char mtext[256];
};

int main() {
    system("touch mqfile");
    key_t key  = ftok("mqfile", 42);
    int  msqid = msgget(key, 0666 | IPC_CREAT);

    struct msgbuf msg;

    if (fork() == 0) {
        // Child: Sleep to ensure parent tries non-blocking receive first
        sleep(2);
        msg.mtype = 2;
        snprintf(msg.mtext, sizeof(msg.mtext), "Late reply from child");
        msgsnd(msqid, &msg, sizeof(msg.mtext), 0);
        exit(0);
    } else {
        // Parent: send initial msg (optional, but D1 had it)
        msg.mtype = 1;
        snprintf(msg.mtext, sizeof(msg.mtext), "Hello child");
        msgsnd(msqid, &msg, sizeof(msg.mtext), 0);

        // Non-blocking receive attempt
        printf("Parent: Attempting non-blocking receive (IPC_NOWAIT)...\n");
        if (msgrcv(msqid, &msg, sizeof(msg.mtext), 2, IPC_NOWAIT) == -1) {
            if (errno == ENOMSG) {
                printf("Parent: No message available in queue (ENOMSG).\n");
            } else {
                perror("msgrcv");
            }
        }

        // Now wait and do blocking receive
        printf("Parent: Now waiting for child and performing blocking receive...\n");
        msgrcv(msqid, &msg, sizeof(msg.mtext), 2, 0);
        printf("Parent: Received reply: %s\n", msg.mtext);

        wait(NULL);
        msgctl(msqid, IPC_RMID, NULL);
    }
    return 0;
}
