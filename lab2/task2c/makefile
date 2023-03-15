all: task2c

task2c :  base.o
	gcc -m32 -g -Wall -32 -g -o 2c base.o

base.o : base.c
	gcc -g -Wall -m32 -c -g -o base.o base.c


.PHONY : clean
clean :
	rm -f *.o 2c
