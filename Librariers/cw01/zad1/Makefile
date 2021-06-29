CC=gcc
CFLAGS=-c -Wall
LIBRARY_NAME=my_library

static :
	$(CC) $(CFLAGS) $(LIBRARY_NAME).c
	ar rcs $(LIBRARY_NAME).a $(LIBRARY_NAME).o

shared :
	$(CC) -fPIC $(CFLAGS) -shared $(LIBRARY_NAME).c -o $(LIBRARY_NAME).so
	
clean :
	rm -f *.o *.so *.a
	
