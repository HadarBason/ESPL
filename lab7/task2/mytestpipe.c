#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>
#include <errno.h>
#include <stdlib.h>

#define STDIN 0
#define STDOUT 1

int main(int argc, char** argv) {
    int debug = 0;
    pid_t child1_pid, child2_pid;
    int i = 0;
    for (; i < argc; i++) { /* is debug mode? */
        if (strcmp(argv[i], "-d") == 0)
            debug = 1;
    }
    int p[2];
    if (pipe(p) == -1) {
        perror("pipe not created\n");
        _exit(1);
    }

    char *argvls[3] = {"ls", "-l", 0}; /* -l flag print the directory in a table format*/
    char *argvtail[4] = {"tail", "-n", "2", 0};/* print 2 last lines of a file*/

    if (debug) {
        fprintf(stderr, "(parent process -> forking)\n");
    }
    if (fork()>0){
        if (debug)
            fprintf(stderr, "(parent process -> created process 1 with id: %d\n", getppid());
        if (debug)
            fprintf(stderr, "(parent process -> closing the writing end of the pipe)\n");
        close(p[1]);
        if (debug) { fprintf(stderr, "(parent process -> waiting for child1 processes to terminate)\n"); }
        wait(NULL);
    }

    if (child1_pid == 0) { /* child 1 */
        if (debug) {
            fprintf(stderr, "(child1 -> redirecting stdout to the writing end of the pipe)\n");
        }
        close(STDOUT);
        dup2(p[1], STDOUT);
        close(p[1]);
        if (debug) {
            fprintf(stderr, "(child1 -> going to execute cmd: ...)\n");
        }
        execvp(argvls[0], argvls);
        perror(strerror(errno));
        _exit(EXIT_FAILURE);
    } else {

        if (debug) {
            fprintf(stderr, "(parent process -> forking)\n");
        }
        child2_pid = fork();
        if (debug)
            fprintf(stderr, "(parent process -> created process 2 with id: %d\n", getppid());
        if (child2_pid == 0) {
            if (debug) {
                fprintf(stderr, "(child2 -> redirecting stdin to the reading end of the pipe)\n");
            }
            close(STDIN);
            dup2(p[0], STDIN);
            close(p[0]);
            if (debug)
                fprintf(stderr, "(child2 -> going to execute cmd: ...)\n");
            execvp(argvtail[0], argvtail);
            perror(strerror(errno));
            _exit(EXIT_FAILURE);
        } else {
            if (debug) { fprintf(stderr, "(parent process -> closing the reading end of the pipe)\n"); }
            close(p[0]);
            if (debug) { fprintf(stderr, "(parent process -> waiting for child1 processes to terminate)\n"); }
            waitpid(child1_pid, NULL, 0);
            if (debug) { fprintf(stderr, "(parent process -> waiting for child2 processes to terminate)\n"); }
            waitpid(child2_pid, NULL, 0);
            if (debug) {
                fprintf(stderr, "(parent process -> closing the reading end of the pipe)\n");
                fprintf(stderr ,"(parent process -> exiting)\n");
            }
        }

    }
    return 0;
}