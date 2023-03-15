
all: task1a

task1a : main.o util.o
	ld -m elf_i386 -g -o task1a main.o
	gcc -g -Wall -o task1a util.o

main.o : main.s 
	nasm -g -f elf -w+all -o main.o main.s
util.o: util.c
	gcc -Wall -ansi -c -nostdlib -fno-stack-protector util.c -o util.o

.PHONY : clean 
clean : 
	rm -f *.o task1a
