#include <stdio.h>
#include <string.h>
#include <stdlib.h>


typedef struct virus {
    unsigned short SigSize;
    char virusName[16];
    unsigned char* sig;
} virus;

typedef struct link link;
struct link {
    link *nextVirus;
    struct virus *vir;
};

struct newMenu{
    char *name;
    link* (*fun)(link*);
};

short bytesToShort(char *bytesArr) {
    short result = (unsigned short) ((bytesArr[1] & 0xff) << 8);
    result += (unsigned short) (bytesArr[0] & 0xff);
    return result;
}


virus *readVirus(FILE* file){
    virus* vir = malloc(sizeof(virus));
    fread(&vir->SigSize, 2, 1, file);
    fread(vir->virusName, 16, 1, file);
    vir->sig = malloc((vir->SigSize) * sizeof(char));
    fread(vir->sig, vir->SigSize, 1, file);
    return vir;

}
void printVirus(virus* vir, FILE* output) {
    fprintf(output, "Virus name: %s \n", vir->virusName);
    fprintf(output, "Virus size: %d \n", vir->SigSize);
    fprintf(output, "Virus signature:\n");
    for (int i = 1; i < vir->SigSize +1; ++i) {
        fprintf(output, "%02X ", vir->sig[i-1]);
        if (i%20 ==0)
            fprintf(output, "\n");
    }
    fprintf(output, "\n\n");
}

void list_print(link *virus_list, FILE* output){
    link* virus = virus_list;
    while (virus) {
        printVirus(virus->vir,output);
        virus = virus->nextVirus;
    }
}

link* list_append(link* virus_list, virus* data){//add virus to the start of the list
    link * newVir = malloc(sizeof (link));
    newVir->vir = data;
    newVir->nextVirus = virus_list;
    return newVir;
}

void list_free(link *virus_list) {
    if (virus_list != NULL) {
        free(virus_list->vir->sig);
        free(virus_list->vir);
        list_free(virus_list->nextVirus);
        free(virus_list);
    }
}
link* load_signatures(link *virusList)
{
    char buffer[200];
    fgets(buffer, sizeof(buffer), stdin);
    size_t length;
    length = strlen(buffer);
    if (buffer[length - 1] == '\n')
        buffer[length - 1] = '\0';

    FILE *file = fopen(buffer, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Error! \n");
        exit(0);
    }
    fseek(file,4,SEEK_SET);

    while (!feof(file)){
        virus  *vir = readVirus(file);
        if(vir->SigSize != 0) {
            virusList = list_append(virusList, vir);
        }else{
            free(vir->sig);
            free(vir);
        }
    }
    fclose(file);
    return virusList;
}

void print_signatures (link *virusList, FILE * output) {
    if (virusList != NULL)
        list_print(virusList, output);
}

link* print_signatures_wrap (link *virusList){
    print_signatures(virusList, stdout);
    return virusList;
}

void detect_virus(char *buffer, unsigned int size, link *virus_list){
    while (virus_list)
    {
        unsigned short sigSize = virus_list->vir->SigSize;
        unsigned int location = 0;
        while ((size - location) - sigSize != 0)
        {
            if (!memcmp(buffer + location, virus_list->vir->sig, sigSize))
            {
                printf("The starting byte location: %d\n", location);
                printf("The virus name: %s\n", virus_list->vir->virusName);
                printf("The size of the virus signature: %d\n\n", sigSize);
            }
                location++;
        }
        virus_list = virus_list->nextVirus;
    }
}
link* detect_virus_wrap(link *virusList){
    printf("please enter file name:\n");
    char buff[200];
    fgets(buff, sizeof(buff), stdin);
    size_t length;
    length = strlen(buff);
    if (buff[length - 1] == '\n')
        buff[length - 1] = '\0';

    FILE *f = fopen(buff, "r");
    if (!f)
    {
        fprintf(stderr, "Error! \n");
        exit(0);
    }
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);
    int min = fsize < 10000 ? fsize : 10000;
    char *buffer = malloc(10000);
    fread(buffer, min, 1, f);
    fclose(f);
    detect_virus(buffer, min, virusList);
    free(buffer);
    return virusList;
}


void fix_file (void ){
    printf("Not implemented \n");
}

link* fix_file_wrap (const link* unused ){
    fix_file();
    return unused;
}
void quit() {
    exit(0);
}

link* quit_wrap (link* virusList ){
    list_free(virusList);
    quit();
    return virusList;
}

int main(int argc, char **argv){
    struct newMenu menu[] = {
            {"Load Signatures",  load_signatures},
            {"Print Signatures", print_signatures_wrap},
            {"Detect viruses",   detect_virus_wrap},
            {"Fix file",             fix_file_wrap },
            {"Quit",                     quit_wrap},
            {NULL, NULL}
    };
    link * virusList = NULL;
    while (1) {
        printf("Please choose a function:\n");
        size_t size = sizeof(menu) == 0 ? 0 : sizeof(menu) / sizeof(menu[0]);
        for (size_t i = 1; i < size; i++)
        {
            printf("%zu) %s\n", i, menu[i-1].name);
        }
        printf("Option:");
        char input[3];
        fgets(input, 3, stdin);

        int num = atoi(&input);
        if ((num < 1) | (num >= size))
        {
            printf("Not within bounds");
            exit(0);
        }
        else {
            printf("Within bounds\n");
            printf("%s\n", menu[num-1].name);
            virusList = menu[num-1].fun(virusList);
        }
        printf("Done.\n");

    }
}