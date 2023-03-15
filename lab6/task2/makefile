all: task2

task2 :  main.o LineParser.o
	gcc -g -Wall -g -o t3 main.o LineParser.o

main.o : main.c
	gcc -g -Wall -c -g -o main.o main.c
LineParser.o : main.c
	gcc -g -Wall -c -g -o LineParser.o LineParser.c


.PHONY : clean
clean :
	rm -f *.o t2
