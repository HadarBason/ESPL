all: task3

task3 :  manu.o
	gcc -g -Wall -g -o t3 manu.o

manu.o : manu.c
	gcc -g -Wall -c -g -o manu.o manu.c


.PHONY : clean
clean :
	rm -f *.o t3
