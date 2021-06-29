CC=gcc
CFLAGS=-c -Wall


static :
	$(CC) $(CFLAGS) main.c
	$(CC) $(CFLAGS) kucharz.c
	$(CC) $(CFLAGS) dostawca.c
	$(CC) $(CFLAGS) functions.c
	ar rcs libfunctions.a functions.o
	$(CC) main.c -L. -l functions -o main
	$(CC) kucharz.c -L. -l functions -o kucharz
	$(CC) dostawca.c -L. -l functions -o dostawca


run : clean static
	./main 8 8
	
clean :
	rm -f *.o *.a kucharz dostawca main