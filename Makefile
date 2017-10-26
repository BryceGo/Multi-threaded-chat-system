all: test

test: main.o Assignment1.o
	gcc -o s-talk -pthread main.o Assignment1.o
	
main.o: main.c Assignment1.h
	gcc -c main.c -o main.o

Assignment1.o: Assignment1.c Assignment1.h
	gcc -o Assignment1.o -c Assignment1.c

clean: 
	rm *.o s-talk
