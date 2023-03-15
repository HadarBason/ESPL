all: task2a

task2a :  base.o
	gcc -g -Wall -g -o 2a base.o

base.o : base.c
	gcc -g -Wall -c -g -o base.o base.c


.PHONY : clean
clean :
	rm -f *.o 2a
