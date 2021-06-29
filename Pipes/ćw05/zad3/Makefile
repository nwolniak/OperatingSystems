CC=gcc
CFLAGS=-c -Wall

PRODUCER_N=4
N=5

static :
	$(CC) $(CFLAGS) main.c
	$(CC) $(CFLAGS) producer.c
	$(CC) $(CFLAGS) consumer.c
	$(CC) producer.o -o producer
	$(CC) consumer.o -o consumer
	$(CC) main.o -o main
	
	
clean :
	rm -f *.o *.a main pipe consumer producer *.output

	
run :
	make clean
	make static
	./main $(PRODUCER_N) $(N)