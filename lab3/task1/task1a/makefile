all: task1a

task1a :  virusDetector.o
	gcc -g -Wall -g -o t1a virusDetector.o

virusDetector.o : virusDetector.c
	gcc -g -Wall -c -g -o virusDetector.o virusDetector.c


.PHONY : clean
clean :
	rm -f *.o t1a
