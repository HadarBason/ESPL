#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define BUFFER_SIZE 32

int main(int argc, char** argv) {
    int p[2];
    char * msg = "hello";
    char buff [BUFFER_SIZE];
    pid_t pid;

    if(pipe(p) == -1){
        perror("pipe does not exist");
        exit(1);
    }
    if((pid = fork())==0){
        close(p[0]);
        write(p[1],msg,BUFFER_SIZE);
        close(p[1]);
    } else{
        close(p[1]);
        read(p[0],buff,BUFFER_SIZE);
        close(p[0]);
        fprintf(stdout,"%s \n",buff);
    }
    return 0;
}