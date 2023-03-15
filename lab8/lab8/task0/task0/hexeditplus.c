#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>


int debug = 0;

typedef struct {
    char debug_mode;
    char file_name[128];
    int unit_size;
    unsigned char mem_buf[10000];
    size_t mem_count;
} state;

typedef struct {
    char *name;
    void (*fun)(state*);
}fun_desc;

void toggleDebugMode(state* s){
    if(debug==0){
        printf("Debug flag now on\n");
        debug=1;
    }
    else{
        printf("Debug flag now off\n");
        debug=0;
    }
}

void setFileName(state* s){
    printf("Enter new File Name: ");
    char newFileName[150];
    fgets(newFileName,150,stdin);
    strcpy(s->file_name,newFileName);
    if(debug){
        printf("Debug: file name set to %s\n", newFileName);
    }
}

int isValid(int n){
    if(n==1 || n==2 || n==4){
        return 1;
    }
    return 0;
}

void setUnitSize (state* s) {
    printf("Enter new Unit Size: \n");
    char input[10];
    int op;
    fgets(input, 10, stdin);
    sscanf(input, "%d\n",&op);
    if (!isValid(op)){
        printf("Invalid number\n");
        return;
    }
    if(debug){
        printf("Debug: set size to %d\n", op);
    }
}

void quit (state* s) {
    if (debug) { printf("quitting..\n");}
    exit(0);
}
void loadIntoMemory(state * s){
    printf("Not implemented yet\n");
}
void toggleDisplayMode(state * s){
    printf("Not implemented yet\n");
}
void memoryDisplay(state * s){
    printf("Not implemented yet\n");
}
void saveIntoFile(state * s){
    printf("Not implemented yet\n");
}void memoryModify(state * s){
    printf("Not implemented yet\n");
}

void display (fun_desc menu[] , state* s){

    fprintf(stdout, "Please choose a function:\n");

    int i=0 ;
    while(menu[i].name != NULL) {
        fprintf(stdout, "%d) %s\n", i, menu[i].name);
        i++;
    }

    fprintf(stdout, "Option: ");
}

int getUserInput (int bounds){
    char input[10];
    int op;
    fgets(input, 10, stdin);
    sscanf(input, "%d\n",&op);

    if (op >= 0 && op < bounds){
        fprintf(stdout, "Within bounds\n" );
        return op;

    }
    else{
        fprintf(stdout, "Not within bounds\n" );
        //exit (0);
        return -1;
    }

}
int main(int argc, char **argv){
    state* s = calloc (1 , sizeof(state));

    fun_desc menu[] = { { "Toggle Debug Mode", toggleDebugMode }, { "Set File Name", setFileName }, { "Set Unit Size", setUnitSize },
                        {"Load Into Memory", loadIntoMemory},{"Toggle Display Mode", toggleDisplayMode},{"Memory Display", memoryDisplay},
                        {"Save Into File",saveIntoFile},{"Memory Modify",memoryModify},{ "Quit", quit } ,{ NULL, NULL } };

    size_t bounds=0;
    while ( menu[bounds].name != NULL){
        bounds = bounds +1;
    }

    while (1) {

        display(menu , s);
        int option = getUserInput (bounds);
        if (option != -1) { menu[option].fun(s); }

        printf("\n");

    }


}
