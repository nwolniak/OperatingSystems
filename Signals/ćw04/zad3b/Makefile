CC=gcc
CFLAGS=-c -Wall

KILL=kill
SIGQUEUE=sigqueue 
SIGRT=sigrt



static :
	$(CC) $(CFLAGS) sender.c
	$(CC) $(CFLAGS) catcher.c
	$(CC) $(CFLAGS) main.c
	$(CC) sender.o -o sender
	$(CC) catcher.o -o catcher
	$(CC) main.o -o main
	
	
clean :
	rm -f *.o *.a main sender catcher

	
run :
	make clean
	make static
	./main 5 $(SIGRT)