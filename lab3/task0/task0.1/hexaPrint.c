#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void printHex(char* buffer, int length){
    for (int i = 0; i < length; ++i)
        printf("%X ",(unsigned char)buffer[i]); // 0-255 instead of -128-127
    printf("\n");
}

int main(int argc, char **argv){
    FILE *file = fopen(argv[1], "r");
    if (!file) {
        fprintf(stderr, "Couldn't find the file!\n");
        exit(0);
    }
    fseek(file, 0, SEEK_END);
    long sizeOfFile = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *buff = malloc(sizeOfFile);
    fread(buff, sizeOfFile, 1, file);
    fclose(file);
    printHex(buff, sizeOfFile);
    free(buff);
}