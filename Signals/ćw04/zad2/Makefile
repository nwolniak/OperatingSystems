CC=gcc
CFLAGS=-c -Wall


static :
	$(CC) $(CFLAGS) main.c
	$(CC) main.o -o main
	
	
clean :
	rm -f *.o *.a main

	
run :
	make clean
	make static
	./main