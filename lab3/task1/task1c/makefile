all: task1c

task1c :  task1c.o
	gcc -g -Wall -g -o t1c task1c.o

task1c.o : task1c.c
	gcc -g -Wall -c -g -o task1c.o task1c.c


.PHONY : clean
clean :
	rm -f *.o t1c
