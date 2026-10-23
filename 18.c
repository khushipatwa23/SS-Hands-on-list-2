
/* Program: Count the directories in the current working directory
   by executing ls -l | grep '^d' | wc -l using two pipes,
   three child processes, and dup2() for input/output redirection. */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    int p1[2], p2[2];

    if (pipe(p1) == -1 || pipe(p2) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t a = fork();

    if (a == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (a == 0) {
        close(p1[0]);
        close(p2[0]);
        close(p2[1]);

        if (dup2(p1[1], STDOUT_FILENO) == -1) {
            perror("dup2 ls");
            _exit(EXIT_FAILURE);
        }

        close(p1[1]);

        execlp("ls", "ls", "-l", (char *)NULL);
        perror("ls");
        _exit(127);
    }

    pid_t b = fork();

    if (b == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (b == 0) {
        close(p1[1]);
        close(p2[0]);

        if (dup2(p1[0], STDIN_FILENO) == -1 ||
            dup2(p2[1], STDOUT_FILENO) == -1) {
            perror("dup2 grep");
            _exit(EXIT_FAILURE);
        }

        close(p1[0]);
        close(p2[1]);

        execlp("grep", "grep", "^d", (char *)NULL);
        perror("grep");
        _exit(127);
    }

    pid_t c = fork();

    if (c == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (c == 0) {
        close(p1[0]);
        close(p1[1]);
        close(p2[1]);

        if (dup2(p2[0], STDIN_FILENO) == -1) {
            perror("dup2 wc");
            _exit(EXIT_FAILURE);
        }

        close(p2[0]);

        execlp("wc", "wc", "-l", (char *)NULL);
        perror("wc");
        _exit(127);
    }

    close(p1[0]);
    close(p1[1]);
    close(p2[0]);
    close(p2[1]);

    waitpid(a, NULL, 0);
    waitpid(b, NULL, 0);
    waitpid(c, NULL, 0);

    return 0;
}
