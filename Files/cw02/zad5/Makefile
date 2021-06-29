CC=gcc
CFLAGS=-c -Wall

TEXT1="text1.txt"
TEXT2="text2.txt"

TEST="test"
LIB="lib"
SYS="sys"
NUMBER_OF_TESTS=10

static :
	$(CC) $(CFLAGS) my_function5.c
	ar rcs libmy_function5.a my_function5.o
	$(CC) $(CFLAGS) main.c
	$(CC) main.c -L. -l my_function5 -o main
	

test :
	make clean
	make static
	./main $(TEST) $(NUMBER_OF_TESTS) $(TEXT1) $(TEXT2)
	
	
run_lib :
	make clean
	make static
	./main $(LIB) 1 $(TEXT1) $(TEXT2)
	

run_sys :
	make clean
	make static
	./main $(SYS) 1 $(TEXT1) $(TEXT2)


clean :
	rm -f *.o *.a main
	
run :
	make clean
	make static
	./main $(TEXT1) $(TEXT2)
