CC=gcc
CFLAGS=-Wall 

static :
	$(CC) $(CFLAGS) -o client client.c -lpthread -lrt
	$(CC) $(CFLAGS) -o server server.c -lpthread -lrt


clean :
	rm -f *.o *.a client server socket


server : clean static
	./server 1024 socket


client1 :
	./client Norbert local socket

client2 :
	./client Marcin local socket
