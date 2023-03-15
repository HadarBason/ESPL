all: exec

exec:  mypipe.o
		gcc -m32 -g -Wall -Wno-nullability-completeness mypipe.o  -o mypipe

mypipe.o: mypipe.c
		gcc -m32 -g -Wall -ansi -c mypipe.c -o mypipe.o

.PHONY: clean
clean:
	rm -f *.o mypipe