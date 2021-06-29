CC=gcc
CFLAGS=-c -Wall

FILE="/home/norbert/Desktop/NorbertWolniak/ćw05/zad1/arguments2.txt"

static :
	$(CC) $(CFLAGS) main.c
	$(CC) main.o -o main
	
	
clean :
	rm -f *.o *.a main

	
run :
	make clean
	make static
	./main $(FILE)
