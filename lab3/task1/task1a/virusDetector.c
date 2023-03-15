#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct virus {
    unsigned short SigSize;
    char virusName[16];
    unsigned char* sig;
} virus;



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



int main(int argc, char **argv){
    FILE* file = fopen("./signatures-L", "rb");
    FILE* output = fopen("./output.txt", "w");
    fseek(file,4,SEEK_SET);
    while (!feof(file)){
        virus *vir;
        vir = readVirus(file);
        if(vir->SigSize != 0) {
            printVirus(vir, output);
            free(vir->sig);
            free(vir);
        }else{
            free(vir->sig);
            free(vir);}
    }
    fclose(file);
    fclose(output);
    return 0;
}