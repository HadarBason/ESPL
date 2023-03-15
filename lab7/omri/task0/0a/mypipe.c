#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>


int main(){

    char buffer;
    int p[2];
    pipe(p);

    if (fork() > 0) {
        write(p[1], "Hello\n", sizeof("Hello"));
        close(p[1]);
        wait(NULL);
    }
    else {
        close(p[1]);
        while (read(p[0], &buffer, 1) != 0)
            putchar(buffer);
        close(p[0]);
    }
    return 0;
}