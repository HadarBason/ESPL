
#include <stdlib.h>
#include <linux/limits.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <x86_64-linux-gnu/sys/wait.h>
#include "LineParser.h"

typedef struct process{
    cmdLine* cmd;                         /* the parsed command line*/
    pid_t pid; 		                  /* the process id that is running the command*/
    int status;                           /* status of the process: RUNNING/SUSPENDED/TERMINATED */
    struct process *next;	                  /* next process in chain */
} process;

#define TERMINATED  -1
#define RUNNING 1
#define SUSPENDED 0

process *processList ;

void addProcess(process** process_list, cmdLine* cmd, pid_t pid){
    process *newProcess = malloc(sizeof (process));
    newProcess->cmd = malloc(sizeof (cmdLine)) ;
    newProcess->cmd = cmd;
    newProcess->pid = pid;
    newProcess->status= 1;
    newProcess->next = *process_list;
    *process_list = newProcess;
}
void printList(process *myProcess){
    fprintf(stdout,"%s\n","im in the list");
    fprintf(stdout, "%s %d ", "index in process list: ", myProcess->cmd->idx);
    fprintf(stdout, "%s %d ", "process id : ", myProcess->pid);
    fprintf(stdout, "%s %d ", "process status: ",myProcess->status);
    fprintf(stdout, "%s %s ", "command: ", myProcess->cmd->arguments[0]);
    for(int i = 1 ;i < myProcess->cmd->argCount; i++){
        fprintf(stdout, "%s %d %s %s ", "arg[" ,i,"]: ", myProcess->cmd->arguments[i]);
    }
    fprintf(stdout,"\n");
}
void printProcessList(process ** process_list) {
    process *currProcess = *process_list;
    while (currProcess!= NULL){
        printList(&currProcess);
        currProcess = currProcess->next;
    }
}
void currDir() {
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        fprintf(stdout,"%s %s\n","Current working dir: ", cwd);
    } else {
        perror("getcwd() error");
    }
}
int isCmdLine (cmdLine *pCmdLine){
    int flag=0;
    if(strcmp(pCmdLine->arguments[0],"cd")==0){
        flag=1;
        if(chdir(pCmdLine->arguments[1])<0)
            perror("cd failed");
    }
    if((strcmp(pCmdLine->arguments[0],"proc")==0)){
        flag=1;
        printProcessList(&processList);
    }
    if(flag == 1)
        freeCmdLines(pCmdLine);
    return flag;
}
void execute(cmdLine *pCmdLine,int debug){
    pid_t child_pid;
    child_pid = fork();
    if(child_pid == 0) {
        if(isCmdLine( pCmdLine)==0) {
            execvp(pCmdLine->arguments[0], pCmdLine->arguments);
            perror("execvp failed \n");
            _exit(0);
        }
        _exit(0);
    }
    else{
        if(debug==1) {
            fprintf(stderr, "%s %d\n", "PID : ", child_pid);
            fprintf(stderr, "%s %s\n", "Executing command : ", pCmdLine->arguments[0]);
        }
        if(pCmdLine->blocking == 001)
            waitpid(child_pid,NULL,0);
    }

}


int main(int argc, char** argv){

    int debug=0;
    for (int i=0; i< argc; i++){
        if (strncmp(argv[i],"-d",2)==0||strncmp(argv[i],"-D",2) == 0)
            debug =1;
    }
    FILE *file = stdin;
    char buff [2048];
    while(1){
        currDir();
        fgets(buff,2048,file);
        cmdLine *pCmdLine = parseCmdLines(buff);
        execute(pCmdLine,debug);
        //freeCmdLines(pCmdLine);
    }
}