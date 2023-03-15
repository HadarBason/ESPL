all: random

random :  random.o
	gcc -g -Wall -g -o random random.o

base.o : base.c
	nasm -f random.s -o random.o

.PHONY : clean
clean :
	rm -f *.o random