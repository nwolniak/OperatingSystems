CC=gcc
CFLAGS=-c -Wall

N=20

run :
	$(CC) $(CFLAGS) main.c -o main.o
	$(CC) main.o -o main
	./main $(N)

clean :
	rm -f *.o main
