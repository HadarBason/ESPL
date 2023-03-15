all: task0.1

task0.1 :  hexaPrint.o
	gcc -g -Wall -g -o t01 hexaPrint.o

hexaPrint.o : hexaPrint.c
	gcc -g -Wall -c -g -o hexaPrint.o hexaPrint.c


.PHONY : clean
clean :
	rm -f *.o t01
