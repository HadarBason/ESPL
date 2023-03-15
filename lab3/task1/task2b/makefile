all: task2b

task2b :  task2b.o
	gcc -g -Wall -g -o t2b task2b.o

task2b.o : task2b.c
	gcc -g -Wall -c -g -o task2b.o task2b.c


.PHONY : clean
clean :
	rm -f *.o t2b
