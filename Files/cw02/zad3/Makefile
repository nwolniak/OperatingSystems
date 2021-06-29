CC=gcc
CFLAGS=-c -Wall

TEST="test"
LIB="lib"
SYS="sys"
NUMBER_OF_TESTS=10

static :
	$(CC) $(CFLAGS) my_function3.c
	ar rcs libmy_function3.a my_function3.o
	$(CC) $(CFLAGS) main.c
	$(CC) main.c -L. -l my_function3 -o main


test :
	make clean
	make static
	./main $(TEST) $(NUMBER_OF_TESTS)


clean :
	rm -f *.o *.a main
	
run_lib :
	make clean
	make static
	./main $(LIB) 1
	

run_sys :
	make clean
	make static
	./main $(SYS) 1
