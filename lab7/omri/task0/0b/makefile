all: exec

exec:  mypipeline.o
		gcc -m32 -g -Wall -Wno-nullability-completeness mypipeline.o  -o mypipeline

mypipeline.o: mypipeline.c
		gcc -m32 -g -Wall -ansi -c mypipeline.c -o mypipeline.o

.PHONY: clean
clean:
	rm -f *.o mypipeline
