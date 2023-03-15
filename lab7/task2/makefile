all: exec

exec:  mypipeline.o
		gcc -g -Wall -Wno-nullability-completeness mypipeline.o  -o mypipeline

mypipeline.o: mypipeline.c
		gcc -g -Wall -ansi -c mypipeline.c -o mypipeline.o

.PHONY: clean
clean:
	rm -f *.o mypipeline
