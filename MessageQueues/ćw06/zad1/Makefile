CC=gcc
CFLAGS=-c -Wall


static :
	$(CC) $(CFLAGS) server.c
	$(CC) $(CFLAGS) client.c
	$(CC) $(CFLAGS) message.c
	ar rcs libmessage.a message.o
	$(CC) server.c -L. -l message -o server
	$(CC) client.c -L. -l message -o client
	
	
clean :
	rm -f *.o *.a server client


s :
	./server


c : client
	./client