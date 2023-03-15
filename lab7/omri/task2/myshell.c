#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "LineParser.h"
#include <sys/types.h>
#include <wait.h>
#include <signal.h>
#include <fcntl.h>

typedef struct process{
    cmdLine* cmd;                       
    pid_t pid;
    int status;                           
    struct process *next;	                  
} process;

#define TERMINATED  -1
#define RUNNING 1
#define SUSPENDED 0
#define STDIN 0
#define STDOUT 1
#define STDERR 2

void addProcess(process** process_list, cmdLine* cmd, pid_t pid);
void printProcessList(process** process_list);
void freeProcessList(process* process_list);
void updateProcessList(process **process_list);
void updateProcessStatus(process* process_list, int pid, int status);
void execute(cmdLine *pCmdLine);
void excutePipe(cmdLine *cmdL);
void redirection(cmdLine* pCmdLine);
int debug;
process* p_list;

int main(int argc, char*argv[]){
    if(argc>1 && strncmp("-d",argv[1],2)==0){
        debug = 1;
    }
    char lastCommand[2048] = {'\0'};
    while(1){
        char directory[PATH_MAX];
        getcwd(directory,PATH_MAX);
        printf("~%s$ > ",directory);
        char read[2048];
        fgets(read,2048,stdin);
        cmdLine* cmdL = parseCmdLines(read);
        if(strncmp("prtrls",cmdL->arguments[0],6) == 0){
            printf("last command was: %s\n", lastCommand);
            freeCmdLines(cmdL);
            continue;
        }
        strncpy(lastCommand, read, 2048);
        if(strncmp("quit",cmdL->arguments[0],4) == 0){
            freeCmdLines(cmdL);
            break;
        }
        if(strncmp("cd",cmdL->arguments[0],2) == 0){
            char newDirectory[PATH_MAX];
            memset(newDirectory,0,PATH_MAX);
            strcat(newDirectory,directory);
            strcat(newDirectory,"/");
            strcat(newDirectory,cmdL->arguments[1]);
            if(chdir(newDirectory) == -1){
                perror("ERROR");
            }
            freeCmdLines(cmdL);
        }
        else if(strncmp("showprocs",cmdL->arguments[0],9) == 0){
            printProcessList(&p_list);
            freeCmdLines(cmdL);
        }
        else {
            execute(cmdL);
        }
    }
    freeProcessList(p_list);
    return 0;
}

void excutePipe(cmdLine *cmdL) {
    int mypipe[2];
    if(pipe(mypipe) == -1){
        perror("Cannot pipe");
        exit(1);
    }
    int child1Id = fork();
    if(child1Id == -1){
        _exit(1);
    }
    if(!child1Id){ //child 1
        close(STDOUT);
        if(dup(mypipe[1]) == -1){
            perror("error ducplicating in child 1 process");
            _exit(1);
        }
        close(mypipe[1]);
        redirection(cmdL);
        if(execvp(cmdL->arguments[0],cmdL->arguments) <0){
            perror("error:");
            _exit(-1);
        }
        exit(1);
    }
    else{ //parent
        close(mypipe[1]);
        int child2Id = fork();
        if(child2Id == -1){
            _exit(1);
        } 
        if(!child2Id){ //child 2
            close(STDIN);
            if(dup(mypipe[0]) == -1){
                perror("error ducplicating in child 2 process");
                _exit(1);
            }
            close(mypipe[0]);
            cmdLine* cmdNext = cmdL->next;
            redirection(cmdNext);
            if(execvp(cmdNext->arguments[0],cmdNext->arguments) <0){
                perror("error:");
                _exit(-1);
            }
            exit(1);
        }
        else{
           close(mypipe[0]);
        }
        if(cmdL->blocking){
            waitpid(child1Id, 0, 0);
        }
        if(child2Id){
            if(cmdL->blocking){
                waitpid(child2Id, 0, 0);
            } 
        } 
    }
}

void redirection(cmdLine* pCmdLine){
    if(pCmdLine->inputRedirect){
        int fdInput = open(pCmdLine->inputRedirect,O_RDONLY,0);
        if(fdInput <0){
            perror("error:");
            exit(-1);
        }
        dup2(fdInput,STDIN);
        close(fdInput);
    }
    if(pCmdLine->outputRedirect){
        int fd = open(pCmdLine->outputRedirect,O_CREAT | O_WRONLY,0777);
        if(fd <0){
            perror("error:");
            exit(-1);
        }
        dup2(fd,STDOUT);
        close(fd);
    }
}


void execute(cmdLine *pCmdLine){
    if(strncmp("nap",pCmdLine->arguments[0],3)==0){
        int p_id = atoi(pCmdLine->arguments[1]);
        int time = atoi(pCmdLine->arguments[2]);
        if(fork() == 0){
            kill(p_id, SIGTSTP);
            sleep(time);
            kill(p_id, SIGCONT);
            freeCmdLines(pCmdLine);
            _exit(1);
        }
        freeCmdLines(pCmdLine);
    }else if(strncmp("stop",pCmdLine->arguments[0],4) == 0){
        int p_id = atoi(pCmdLine->arguments[1]);
        kill(p_id, SIGINT);
        updateProcessStatus(p_list,p_id, TERMINATED);
        freeCmdLines(pCmdLine);
    }if (pCmdLine->next){
        excutePipe(pCmdLine);
    }else{
        int pid;
        if((pid = fork()) <0){
            freeCmdLines(pCmdLine);
            _exit(1);
        }
        else if(!pid){  //child
            if(debug)
                fprintf(stderr,"Child PID: [%d]\n",getpid());
            else if(execvp(pCmdLine->arguments[0],pCmdLine->arguments) < 0){
                perror("ERROR");
                freeCmdLines(pCmdLine);
                _exit(1);
            }
        }
        else  //parent
        {
            if(debug){
                fprintf(stderr,"Parent PID: [%d]\n",getpid());
                fprintf(stderr,"Command: [%s]\n",pCmdLine->arguments[0]);           
            }
            addProcess(&p_list,pCmdLine,pid);
            if(pCmdLine-> blocking){
                waitpid(pid,NULL,0);
            }
        }
    }
}

void addProcess(process** process_list, cmdLine* cmd, pid_t pid){
    process* new_proccess = malloc(sizeof(struct process));
    new_proccess->cmd = cmd;
    new_proccess->pid = pid;
    new_proccess->status = RUNNING;
    if(*process_list == NULL){
        new_proccess->next = NULL;
        *process_list = new_proccess;
    } else{
    new_proccess->next = *process_list;
    *process_list = new_proccess;
    }
}

void printProcessList(process** process_list){
    printf("PID\t\tCommand\t\tSTATUS\n");
    process* p = *process_list;
    updateProcessList(process_list);
    process *last = NULL;
    process *head = *process_list;
    while (p!=NULL)
    {
        char* status = (p)->status == TERMINATED? "TERMINATED":
                       (p)->status == RUNNING?"RUNNING":
                       "SUSPENDED";
        printf("%d\t\t%s\t\t%s\n",(p)->pid,(p)->cmd->arguments[0],status);
        process* next = p->next;
        if(p->status == TERMINATED){
            if(last!=NULL){
                last->next = next;
            }
            else
                head = next;
            freeCmdLines(p->cmd);
            free(p);
        }else
            last = p;
        p = (p)->next;        
    }
    p_list = head;
}

void updateProcessStatus(process* process_list, int pid, int status){
    while(process_list!=NULL){
        if(process_list->pid == pid){
            process_list->status = status;
            break;
        }
        process_list=process_list->next;
    }
}

void freeProcessList(process* process_list){
    while(process_list != NULL){
        process* next = process_list->next;
        freeCmdLines(process_list->cmd);
        free(process_list);
        process_list = next;
    }
}

void updateProcessList(process **process_list){
    process* p = *process_list;
    int status;
    pid_t child_pid;
    while(p!=NULL){
        child_pid = waitpid(p->pid, &status,WNOHANG | WUNTRACED | WCONTINUED);
        if(child_pid == TERMINATED){
            updateProcessStatus(*process_list,p->pid,TERMINATED);
        }
        if(child_pid >0){
            if (WIFEXITED(status) || WIFSIGNALED(status)) {
                updateProcessStatus(*process_list, p->pid, TERMINATED);
            }
            else if(WIFCONTINUED(status)){
                updateProcessStatus(*process_list, p->pid, RUNNING);
            }
            else if(WIFSTOPPED(status)){
                updateProcessStatus(*process_list, p->pid, SUSPENDED);
            }
        }
        p = p->next;
    }
}