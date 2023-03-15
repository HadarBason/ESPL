all: task0

task0 :  bubblesort.o
	gcc -g -Wall -g -o t0 bubblesort.o

bubblesort.o : bubblesort.c
	gcc -g -Wall -c -g -o bubblesort.o bubblesort.c


.PHONY : clean
clean :
	rm -f *.o t0
