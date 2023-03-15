#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>


typedef struct {
    char debug_mode;
    char hex_mode;
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
    if(s->debug_mode==0){
        printf("Debug flag now on\n");
        s->debug_mode=1;
    }
    else{
        printf("Debug flag now off\n");
        s->debug_mode=0;
    }
}

void setFileName(state* s){
    printf("Enter new File Name: ");
    char newFileName[150];
    fgets(newFileName,150,stdin);
    sscanf(newFileName,"%s\n",s->file_name);
    if(s->debug_mode){
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
    } else{
        s->unit_size=op;
    }
    if(s->debug_mode){
        printf("Debug: set size to %d\n", op);
    }
}

void quit (state* s) {
    if (s->debug_mode) { printf("quitting..\n");}
    exit(0);
}
void loadIntoMemory(state * s){

    if(strcmp(s->file_name,"")==0) {
        fprintf(stdout, "%s", "file name do not exits");
        return;
    }
    FILE* file = fopen(s-> file_name, "r+");
    if(file == NULL){
        fprintf(stdout, "%s", "file do not exits");
        return;
    }
    char location[20];
    char length[20];
    int place;
    int len;
    fprintf(stdout,"%s", "please enter <location> <length> :\n");
    char input[50];
    for(int i = 0 ; i < 50 ; i++){
        input[i] = '0';
    }
    int op;
    fgets(input, 10, stdin);
    sscanf(input, "%x %d\n",&place,&len);

    if(s->debug_mode==1){
        fprintf(stdout,"%s %s\n %s %X\n %s %d\n","file name: " ,s->file_name,"location: ",place,"length: ",len);
    }
    fseek (file, place, SEEK_SET);
    s->mem_count = len*(s->unit_size);
    fread(s->mem_buf,s->unit_size,len,file);
    fclose(file);
    fprintf(stdout,"Loaded %d units into memory\n",len);
}
void toggleDisplayMode(state * s){
    if(s->debug_mode==0){
        printf("Display flag now on, hexadecimal representation\n");
        s->hex_mode=1;
    }
    else{
        printf("Display flag now off, decimal representation\n");
        s->hex_mode=0;
    }
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
        if (s->hex_mode == 0){
            fprintf(stdout, "%d) %s\n", i, menu[i].name);
            i++;
        }
        else{
            fprintf(stdout, "%X) %s\n", i, menu[i].name);
            i++;
        }
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
