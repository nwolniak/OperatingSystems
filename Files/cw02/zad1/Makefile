CC=gcc
CFLAGS= -c -Wall
TEXT1="textA.txt"
TEXT2="textB.txt"

TEST="test"
LIB="lib"
SYS="sys"
NUMBER_OF_TESTS=100


static :
	$(CC) $(CFLAGS) my_function1.c
	ar rcs libmy_function1.a my_function1.o
	$(CC) $(CFLAGS) main.c
	$(CC) main.c -L. -l my_function1 -o main


test :
	make clean
	make static
	./main $(TEST) $(NUMBER_OF_TESTS) $(TEXT1) $(TEXT2)


clean :
	rm -f *.o *.a main


	
run_lib :
	make clean
	make static
	./main $(LIB) 1 $(TEXT1) $(TEXT2)


run_sys :
	make clean
	make static
	./main $(SYS) 1 $(TEXT1) $(TEXT2)

