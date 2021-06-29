CC=gcc
CFLAGS=-c -Wall

NADAWCA="nadawca"
DATA="data"
EMAIL="norbert@ubuntu"
TITLE="Hej"
TRESC="abcdabcd"

static :
	$(CC) $(CFLAGS) main.c
	$(CC) main.o -o main
	
	
clean :
	rm -f *.o *.a main

	
run :
	make clean
	make static
	#./main $(NADAWCA)
	./main $(EMAIL) $(TITLE) $(TRESC)
