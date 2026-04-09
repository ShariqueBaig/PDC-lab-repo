#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int fd[2];
    if (pipe(fd) == -1) {
        perror("pipe");
        return 1;
    }

    pid_t pid1 = fork();
    if (pid1 == 0) {
        // Child 1: runs 'ls'
        // Redirect stdout to pipe write end
        dup2(fd[1], STDOUT_FILENO);
        close(fd[0]);
        close(fd[1]);
        execlp("ls", "ls", NULL);
        perror("execlp ls");
        return 1;
    }

    pid_t pid2 = fork();
    if (pid2 == 0) {
        // Child 2: runs 'wc -l'
        // Redirect stdin to pipe read end
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        close(fd[1]);
        execlp("wc", "wc", "-l", NULL);
        perror("execlp wc");
        return 1;
    }

    // Parent closes both ends and waits
    close(fd[0]);
    close(fd[1]);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    return 0;
}
