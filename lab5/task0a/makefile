all: task0	

task0: start.o main.o util.o
	ld -m elf_i386 -o task0 start.o main.o util.o   
start.o: start.s	
	nasm -f elf32 start.s -o start.o
main.o: main.c
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector main.c -o main.o 
util.o: util.c
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector util.c -o util.o

.PHONY : clean 
clean:
	rm -f *.o task0