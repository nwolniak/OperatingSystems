CC=gcc
CFLAGS=-c -Wall

DIR=./
OTHER_DIR=/home/
PATTERN="podany"
MAX_DEPTH=5

static :
	$(CC) $(CFLAGS) my_library.c
	ar rcs libmy_library.a my_library.o
	$(CC) $(CFLAGS) main.c
	$(CC) main.c -L. -l my_library -o main
	
	
clean :
	rm -f *.o *.a main

	
run :
	make clean
	make static
	./main $(DIR) $(PATTERN) $(MAX_DEPTH)
