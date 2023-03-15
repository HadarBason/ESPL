all: task0b

task0b : main.o util.o
	gcc -m32 -g -Wall -o task0b main.o util.o

main.o : main.s 
	nasm -g -f elf -w+all -o main.o main.s
util.o: util.c
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector util.c -o util.o

.PHONY : clean 
clean : 
	rm -f *.o task0b
