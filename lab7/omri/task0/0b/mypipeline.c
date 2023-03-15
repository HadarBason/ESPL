#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char **argv){
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
    char* argv2[4] = { "tail","-n", "2", 0 };
    int arr[2]= {0,0};
    if(debug){
        fprintf(stderr ,"(parent process -> forking)\n");
    }

    if (fork() > 0) { /* parent */
        if(debug){
            fprintf(stderr ,"(parent process -> created process with id: %d\n",getppid());
            fprintf(stderr ,"(parent process -> closing the writing end of the pipe)\n");
        }

        close(p[1]);
        arr[1]=1;
        if(debug){
            fprintf(stderr ,"(parent process -> closing the reading end of the pipe)\n");
        }
        close(p[0]);
        if(debug){
            fprintf(stderr ,"(parent process -> waiting for child processes to terminate)");
        }
        wait(NULL);
    }
    else { /* child 1 */

        close(p[1]);
        if(debug){
            fprintf(stderr ,"(child1 -> redirecting stdout to the writing end of the pipe)");
        }
        p2[1] = dup(p[1]);
        close(p[1]);
        if(debug){
            fprintf(stderr ,"(child1 -> going to execute cmd: ...)");
        }
        execvp(argv1[0], argv1);

        if(fork() == 0){ /* child 2 */
            close(p[1]);
            if(debug){
                fprintf(stderr ,"(child2 -> redirecting stdin to the reading end of the pipe)");
            }
            p2[0] = dup(p[0]);
            close(p[1]);
            if(debug){
                fprintf(stderr ,"(child2 -> going to execute cmd: ...)");
            }
            execvp(argv2[0], argv2);
        }else{
            close(p[0]);
            arr[0]=1;
        }
        while (arr[0]==0 || arr[1]==0){
            wait(NULL);
        }
    }
    if(debug){
        fprintf(stderr ,"(parent process -> exiting)\n");
    }



    return 0;
}

