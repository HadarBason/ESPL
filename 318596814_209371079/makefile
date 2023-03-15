all: task2 task1 task3
task1:task1.c
	gcc -g -m32 -fno-pie -c -o task1.o task1.c
	gcc -g -m32 task1.o -o task1
	
task2:task2.c
	gcc -g -m32 -fno-pie -c -o task2.o task2.c
	gcc -g -m32 task2.o -o task2
	
task3:task3.c
	gcc -g -m32 -fno-pie -c -o task3.o task3.c
	gcc -g -m32 task3.o -o task3
.PHONY: clean
clean:
	rm -rf ./*.o task2 task1 task3
