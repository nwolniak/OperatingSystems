CC=gcc
CFLAGS=-c -Wall

IGNORE="ignore"
HANDLER="handler"
MASK="mask"
PENDING="pending"

static :
	$(CC) $(CFLAGS) main.c
	$(CC) $(CFLAGS) next_child.c
	$(CC) next_child.o -o next_child
	$(CC) main.o -o main
	
	
clean :
	rm -f *.o *.a main next_child

	
run :
	make clean
	make static
	./main $(IGNORE)