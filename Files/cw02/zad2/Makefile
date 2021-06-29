CC=gcc
CFLAGS=-c -Wall

CHAR="c"
TEXT="text.txt"

TEST="test"
LIB="lib"
SYS="sys"
NUMBER_OF_TESTS=10

static :
	$(CC) $(CFLAGS) my_function2.c
	ar rcs libmy_function2.a my_function2.o
	$(CC) $(CFLAGS) main.c
	$(CC) main.c -L. -l my_function2 -o main


test :
	make clean
	make static
	./main $(TEST) $(NUMBER_OF_TESTS) $(CHAR) $(TEXT)


clean :
	rm -f *.o *.a main
	
run_lib :
	make clean
	make static
	./main $(LIB) 1 $(CHAR) $(TEXT)
	

run_sys :
	make clean
	make static
	./main $(SYS) 1 $(CHAR) $(TEXT)
