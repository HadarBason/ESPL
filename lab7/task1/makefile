all: edit mypipeline

edit :  task2.o LineParser.o
	gcc -m32 -g -Wall -o task2 task2.o LineParser.o

task2.o : task2.c 
	gcc -g -Wall -m32 -c -o task2.o task2.c

LineParser.o : LineParser.c 
	gcc -g -Wall -m32 -c -o LineParser.o LineParser.c
	
mypipeline: mypipeline.o
	gcc -m32 -g -Wall -o mypipeline mypipeline.o
mypipeline.o : mypipeline.c
	gcc -g -Wall -m32 -c -o mypipeline.o mypipeline.c

.PHONY : clean 
clean : 
	rm -f *.o task2
