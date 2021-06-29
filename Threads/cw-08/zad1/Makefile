CC=gcc
CFLAGS=-Wall 


static :
	$(CC) $(CFLAGS) -o main main.c -lpthread -lrt


run : clean static
	./main 16 block dla.ascii.pgm dla_out.ascii.pgm

test : clean static
	./main 1 numbers feep.ascii.pgm feep_out.ascii.pgm
	./main 2 numbers feep.ascii.pgm feep_out.ascii.pgm
	./main 4 numbers feep.ascii.pgm feep_out.ascii.pgm
	./main 8 numbers feep.ascii.pgm feep_out.ascii.pgm
	./main 16 numbers feep.ascii.pgm feep_out.ascii.pgm
	./main 1 block feep.ascii.pgm feep_out.ascii.pgm
	./main 2 block feep.ascii.pgm feep_out.ascii.pgm
	./main 4 block feep.ascii.pgm feep_out.ascii.pgm
	./main 8 block feep.ascii.pgm feep_out.ascii.pgm
	./main 16 block feep.ascii.pgm feep_out.ascii.pgm

	./main 1 numbers pepper.ascii.pgm pepper_out.ascii.pgm
	./main 2 numbers pepper.ascii.pgm pepper_out.ascii.pgm
	./main 4 numbers pepper.ascii.pgm pepper_out.ascii.pgm
	./main 8 numbers pepper.ascii.pgm pepper_out.ascii.pgm
	./main 16 numbers pepper.ascii.pgm pepper_out.ascii.pgm
	./main 1 block pepper.ascii.pgm pepper_out.ascii.pgm
	./main 2 block pepper.ascii.pgm pepper_out.ascii.pgm
	./main 4 block pepper.ascii.pgm pepper_out.ascii.pgm
	./main 8 block pepper.ascii.pgm pepper_out.ascii.pgm
	./main 16 block pepper.ascii.pgm pepper_out.ascii.pgm

	./main 1 numbers baboon.ascii.pgm baboon_out.ascii.pgm
	./main 2 numbers baboon.ascii.pgm baboon_out.ascii.pgm
	./main 4 numbers baboon.ascii.pgm baboon_out.ascii.pgm
	./main 8 numbers baboon.ascii.pgm baboon_out.ascii.pgm
	./main 16 numbers baboon.ascii.pgm baboon_out.ascii.pgm
	./main 1 block baboon.ascii.pgm baboon_out.ascii.pgm
	./main 2 block baboon.ascii.pgm baboon_out.ascii.pgm
	./main 4 block baboon.ascii.pgm baboon_out.ascii.pgm
	./main 8 block baboon.ascii.pgm baboon_out.ascii.pgm
	./main 16 block baboon.ascii.pgm baboon_out.ascii.pgm

	./main 1 numbers dla.ascii.pgm dla_out.ascii.pgm
	./main 2 numbers dla.ascii.pgm dla_out.ascii.pgm
	./main 4 numbers dla.ascii.pgm dla_out.ascii.pgm
	./main 8 numbers dla.ascii.pgm dla_out.ascii.pgm
	./main 16 numbers dla.ascii.pgm dla_out.ascii.pgm
	./main 1 block dla.ascii.pgm dla_out.ascii.pgm
	./main 2 block dla.ascii.pgm dla_out.ascii.pgm
	./main 4 block dla.ascii.pgm dla_out.ascii.pgm
	./main 8 block dla.ascii.pgm dla_out.ascii.pgm
	./main 16 block dla.ascii.pgm dla_out.ascii.pgm
	
clean :
	rm -f *.o *.a main Times.txt