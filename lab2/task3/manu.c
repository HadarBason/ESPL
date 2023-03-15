#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char my_get(char c){ /* Ignores c, reads and returns a character from stdin using fgetc. */
    return getc(stdin);
}


char cprt(char c){/* If c is a number between 0x20 and 0x7E, cprt prints the character of ASCII value c followed by a new line. Otherwise, cprt prints the dot ('.') character. After printing, cprt returns the value of c unchanged. */
    if (c >=0x20 && c <= 0x7E )
        printf("%c \n", c);
    else
        printf("%c \n", '.');
    return c;
}

char encrypt(char c){/* Gets a char c and returns its encrypted form by adding 3 to its value. If c is not between 0x20 and 0x7E it is returned unchanged */
    if (c >= 0x20 && c <= 0x7E )
        return c+3;
    else
        return c;
}


char decrypt(char c){/* Gets a char c and returns its decrypted form by reducing 3 to its value. If c is not between 0x20 and 0x7E it is returned unchanged */
    if (c >= 0x20 && c <= 0x7E )
        return c-3;
    else
        return c;
}

char xprt(char c){/* xprt prints the value of c in a hexadecimal representation followed by a new line, and returns c unchanged. */
    if (c >=0x20 && c <= 0x7E )
        printf("%x \n", c);
    else
        printf("%c \n", '.');
    return c;
}
char quit(char c){/* Gets a char c, and if the char is 'q' , ends the program with exit code 0. Otherwise returns c. */
    if (c == 'q')
        exit(0);
}


char censor(char c) {
    if(c == '!')
        return '.';
    else
        return c;
}

struct fun_desc {
    char *name;
    char (*fun)(char);
};

char* map(char *array, int array_length, char (*f) (char)){
    char* mapped_array = (char*)(malloc(array_length*sizeof(char)));
    for(int i = 0 ; i< array_length; i++){
        mapped_array[i]= f(array[i]);
    }
    return mapped_array;
}

int main(int argc, char **argv){
    char array[5] = {'\0'};
    char *carray = array;
    int option;
    char input[10];
    struct fun_desc menu[] = { { "Get string", my_get }, { "Print string", cprt }, { "Print hex", xprt }, { "Censor", censor },   { "Encrypt", encrypt }, { "Decrypt", decrypt },
                                {"Quit",quit},{ NULL, NULL } };
    int menulen = (int)(sizeof(menu)/sizeof(menu[0]))-1;
    while(1) {
        printf("Please choose a function: \n");
        for (int i = 0; i < (int) (sizeof(menu) / sizeof(menu[0])) - 1; i++) {
            printf("%d. %s \n", i, menu[i].name);
        }
        fgets(input, 10, stdin);
        sscanf(input, "%d\n",&option);
        if (option >= 0 && option <= menulen - 1) {
            printf("Option : %d \n\n", option);
            printf("Within bounds \n");
            carray = map(carray,5,menu[option].fun);
            printf("DONE. \n");
        } else {
            printf("Not within bounds\n");
            exit(0);
        }
    }


}
