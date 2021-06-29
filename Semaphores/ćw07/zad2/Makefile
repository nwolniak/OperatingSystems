CC=gcc
CFLAGS=-Wall 


static :
	$(CC) $(CFLAGS) -o main main.c -lpthread -lrt
	$(CC) $(CFLAGS) -o kucharz kucharz.c -lpthread -lrt
	$(CC) $(CFLAGS) -o dostawca dostawca.c -lpthread -lrt


run : clean static
	./main 6 6
	
clean :
	rm -f *.o *.a kucharz dostawca main