CC=gcc
CFLAGS=-Wall 


static :
	$(CC) $(CFLAGS) -o main main.c -lpthread -lrt

clean :
	rm -f *.o *.a main

run : clean static
	./main

