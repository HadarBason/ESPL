#include "LineParser.h"
#include <stdlib.h>
#include <linux/limits.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <x86_64-linux-gnu/sys/wait.h>
#include <errno.h>
#include <fcntl.h>


typedef struct process{
    cmdLine* cmd;                         /* the parsed command line*/
    pid_t pid; 		                  /* the process id that is running the command*/
    int status;                           /* status of the process: RUNNING/SUSPENDED/TERMINATED */
    struct process *next;	                  /* next process in chain */
} process;

process* GpList;
void free_list(process* process_list){
    process* curr_process=process_list;
    if(curr_process!=NULL){
        free_list(curr_process->next);
        freeCmdLines(curr_process->cmd);
        free(curr_process->cmd);
        free(curr_process);
    }
}
void updateProcessStatus(process* process_list, int pid, int status){
    if (process_list->pid == pid)
        process_list->status=status;
    else{
        while(process_list!=NULL){
            if(process_list->pid == pid) {
                process_list->status = status;
                break;
            }
            process_list=process_list->next;
        }

    }
}

void updateProcessList(process **process_list){
    process* curr_process = (*process_list);
    while(curr_process!=NULL){
        int status;
        int updatedStatus=1; //running
        int wait = waitpid(curr_process->pid,&status,WNOHANG);
        if(wait!=0){    //status changed
            if(WIFEXITED(status) || WIFSIGNALED(status))
                updatedStatus=-1;// terminated
            else if(WIFSTOPPED(status))
                updatedStatus=0; //suspended
            else if(WIFCONTINUED(status))
                updatedStatus=1;//running
            updateProcessStatus(curr_process,curr_process->pid,updatedStatus);
        }
        curr_process=curr_process->next;
    }
}

void addProcess(process** process_list, cmdLine* cmd, pid_t pid){
    process *newProcess = malloc(sizeof (process));
    newProcess->cmd = cmd;
    if(*process_list != NULL)
        newProcess->cmd->idx = ((*process_list)->cmd->idx)+1;
    newProcess->pid = pid;
    newProcess->status= 1;
    newProcess->next = *process_list;
    *process_list = newProcess;
}
void deleteProcess(process *currProcess){
    process *prevProcess = GpList;
    if(prevProcess->pid == currProcess->pid){
        GpList = currProcess->next;
    }
    else {
        while (prevProcess->next != NULL && prevProcess->next->pid != currProcess->pid) {
            prevProcess = prevProcess->next;
        }
        prevProcess->next = currProcess->next;
    }
    freeCmdLines(currProcess->cmd);
    free(currProcess);
}

char* getStatusString(int status){
    if(status == -1)
        return "Terminated";
    if(status == 0)
        return "Suspended";
    else
        return "Running";
}
void printProcess(process *myProcess){
    fprintf(stdout, "%s %d", "index in process list: ", myProcess->cmd->idx);
    fprintf(stdout, "%s %d", ", process id : ", myProcess->pid);
    fprintf(stdout, "%s %s", ", process status: ",getStatusString(myProcess->status));
    fprintf(stdout, "%s %s", ", command: ", myProcess->cmd->arguments[0]);
    for(int i = 1 ;i < myProcess->cmd->argCount; i++){
        fprintf(stdout, "%s %d %s %s", ", arg[",i,"]:", myProcess->cmd->arguments[i]);
    }
    fprintf(stdout,"\n");
}
void printProcessList(process ** process_list) {
    process *currProcess = *process_list;
    updateProcessList(process_list);
    while (currProcess!= NULL){
        printProcess(currProcess);
        if(currProcess->status==-1)
            deleteProcess(currProcess);
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
    if(strcmp(pCmdLine->arguments[0],"quit")==0){
        flag=1;
        free_list(GpList);
        exit(0);
    }
    if((strcmp(pCmdLine->arguments[0],"procs")==0)){
        flag=1;
        printProcessList(&GpList);
    }
    if(strcmp(pCmdLine->arguments[0],"kill")==0){
        flag=1;
        int pid = atoi(pCmdLine->arguments[1]);
        if(kill(pid,SIGINT)==-1)    //terminate
            perror(strerror(errno));
    }
    if(strcmp(pCmdLine->arguments[0],"suspend")==0){
        flag=1;
        int pid = atoi(pCmdLine->arguments[1]);
        if(kill(pid,SIGSTOP)==-1)    //terminate
            perror(strerror(errno));
        else
            updateProcessStatus(GpList, pid, 0);
    }
    if(strcmp(pCmdLine->arguments[0],"wake")==0){
        flag=1;
        int pid = atoi(pCmdLine->arguments[1]);
        if(kill(pid,SIGCONT)==-1)
            perror(strerror(errno));
        else
            updateProcessStatus(GpList, pid, 1);
    }
    if(flag)
        freeCmdLines(pCmdLine);
    return flag;
}
void execute(cmdLine *pCmdLine,int debug) {
    int input = 0;
    int output = 1;
    if (isCmdLine(pCmdLine) == 0) {
        pid_t child_pid;
        child_pid = fork();
        if (child_pid == 0) {
        if (pCmdLine->inputRedirect) {
            close(input);
            input = open(pCmdLine->inputRedirect, O_RDONLY);
            if (input < 0) {
                perror("input unavailable");
                exit(1);
            }
        }
        if (pCmdLine->outputRedirect) {
            close(output);
            output = open(pCmdLine-> outputRedirect, O_WRONLY | O_CREAT);
            fprintf(stdout,"%d %s \n",output,"in out");
            if (output < 0) {
                perror("output unavailable");
                exit(1);
            }
        }
            execvp(pCmdLine->arguments[0], pCmdLine->arguments);
            perror("execvp failed \n");
            _exit(0);
        } else {
            addProcess(&GpList, pCmdLine, child_pid);
            if (debug == 1) {
                fprintf(stderr, "%s %d\n", "PID : ", child_pid);
                fprintf(stderr, "%s %s\n", "Executing command : ", pCmdLine->arguments[0]);
            }
            if (pCmdLine->blocking == 001)
                waitpid(child_pid, NULL, 0);
        }

    }
}

int main(int argc, char** argv){
    GpList= NULL;
    int debug=0;
    for (int i=0; i< argc; i++){
        if (strncmp(argv[i],"-d",2)==0||strncmp(argv[i],"-D",2) == 0)
            debug =1;
    }
    FILE *file = stdin;
    char buff [2048];
    cmdLine *pCmdLine;
    while(1){
        currDir();
        fgets(buff,2048,file);
        if(feof(file))
            exit(0);
        pCmdLine = parseCmdLines(buff);
        if(pCmdLine != NULL)
            execute(pCmdLine, debug);

    }
}















