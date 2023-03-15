all: task1b

task1b :  virusDetector.o
	gcc -g -Wall -g -o t1b virusDetector.o

virusDetector.o : virusDetector.c
	gcc -g -Wall -c -g -o virusDetector.o virusDetector.c


.PHONY : clean
clean :
	rm -f *.o t1b
