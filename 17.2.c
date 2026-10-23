
/* Program: Execute ls -l | wc using pipe(), fork(), and dup2().
   The first child redirects stdout to the pipe and executes ls -l.
   The second child redirects stdin from the pipe and executes wc. */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    int fd[2];

    if (pipe(fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t p1 = fork();

    if (p1 == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (p1 == 0) {
        close(fd[0]);

        if (dup2(fd[1], STDOUT_FILENO) == -1) {
            perror("dup2 stdout");
            _exit(EXIT_FAILURE);
        }

        close(fd[1]);

        execlp("ls", "ls", "-l", (char *)NULL);
        perror("execlp ls");
        _exit(127);
    }

    pid_t p2 = fork();

    if (p2 == -1) {
        perror("fork");
        close(fd[0]);
        close(fd[1]);
        waitpid(p1, NULL, 0);
        exit(EXIT_FAILURE);
    }

    if (p2 == 0) {
        close(fd[1]);

        if (dup2(fd[0], STDIN_FILENO) == -1) {
            perror("dup2 stdin");
            _exit(EXIT_FAILURE);
        }

        close(fd[0]);

        execlp("wc", "wc", (char *)NULL);
        perror("execlp wc");
        _exit(127);
    }

    close(fd[0]);
    close(fd[1]);

    waitpid(p1, NULL, 0);
    waitpid(p2, NULL, 0);

    return 0;
}
