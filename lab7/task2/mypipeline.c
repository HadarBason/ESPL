#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>
#include <errno.h>
#include <stdlib.h>
#define STDIN 0
#define STDOUT 1
int main(int argc, char** argv){
    int pi_d ;
    int pid ;
    int i=0;
    int debug = 0;
    for(; i < argc; i++){
        if(strcmp(argv[i], "-d") == 0)
            debug = 1;
    }
    int p[2];
    int p2[2];
    pipe(p);
    char* argv1[3] = { "ls","-l", 0 };
    char* argv2[4] = {"tail","-n", "2","makefile",0 };

    if(debug){fprintf(stderr ,"(parent process -> forking)\n");}
    pi_d = fork();

    if(pi_d == 0){ /* child 1 */
        if(debug) {fprintf(stderr, "(parent process -> created process with id: %d\n", getpid());}
        if(debug) {fprintf(stderr, "(child1>redirecting stdout to the write end of the pipe…)\n");}
        close(p[1]);
        p2[1] = dup(p[1]);
        close(p[1]);
        if(debug) {fprintf(stderr ,"(child1>going to execute cmd: …)\n");}
        execvp(argv1[0], argv1);
        _exit(1);
    }
    if(pi_d > 0){/* parent */
        wait(NULL);
        if(debug){fprintf(stderr ,"(parent process -> forking)\n");}
        pid = fork();

        if(pid > 0){ /* parent */
            if(debug) {fprintf(stderr ,"(parent process -> closing the write end of the pipe)\n");}
            close(p[1]);
            if(debug) {fprintf(stderr ,"(parent process -> closing the read end of the pipe)\n");}
            close(p[0]);
            if(debug) {fprintf(stderr ,"(parent_process>waiting for child processes to terminate…)\n");}
            wait(NULL);
            if(debug) {fprintf(stderr ,"(parent_process>exiting…)\n");}
            return 0;
        }
        else if(pid == 0){ /* child 2 */
            if(debug) {fprintf(stderr, "(parent process -> created process with id: %d\n", getpid());}
            if(debug) {fprintf(stderr, "(child2>redirecting stdin to the read end of the pipe…)\n");}
            close(p[1]);
            p2[0] = dup(p[0]);
            close(p[0]);
            if(debug) {fprintf(stderr ,"(child2>going to execute cmd: …)\n");}
            execvp(argv2[0], argv2);
            if(debug) {fprintf(stderr ,"(child2>tail not work)\n");}
            _exit(1);
        }

    }
    return 0;
}