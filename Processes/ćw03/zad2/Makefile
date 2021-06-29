CC=gcc
CFLAGS=-c -Wall

SMALL="smallA.txt:smallB.txt"
MEDIUM="mediumA.txt:mediumB.txt"
BIG="bigA.txt:bigB.txt"

static :
	$(CC) $(CFLAGS) my_library.c
	ar rcs libmy_library.a my_library.o
	$(CC) $(CFLAGS) main.c
	$(CC) main.c -L. -l my_library -o main
	

test :
	make clean_raport
	make static
	echo "Test : " >> raport.txt
	make merge_few_small_pairs
	make merge_more_small_pairs
	make merge_many_small_pairs
	make merge_few_medium_pairs
	make merge_more_medium_pairs
	make merge_many_medium_pairs
	make merge_few_big_pairs
	make merge_more_big_pairs
	make merge_many_big_pairs
	

clean :
	rm -f *.o *.a main

clean_raport:
	rm -f *.o *.a main raport.txt
	
run :
	make clean_raport
	make static
	./main 10 merge_files $(SMALL)
	
merge_few_small_pairs :
	echo "merge_few_small_pairs results :" >> raport.txt
	./main 3 merge_files $(SMALL)
	
merge_more_small_pairs :
	echo "merge_more_small_pairs results :" >> raport.txt
	./main 11 merge_files $(SMALL)
	
merge_many_small_pairs :
	echo "merge_many_small_pairs results :" >> raport.txt
	./main 43 merge_files $(SMALL)

merge_few_medium_pairs :
	echo "merge_few_medium_pairs results :" >> raport.txt
	./main 3 merge_files $(MEDIUM)
	
merge_more_medium_pairs:
	echo "merge_more_medium_pairs results :" >> raport.txt
	./main 11 merge_files $(MEDIUM)
	
merge_many_medium_pairs :
	echo "merge_many_medium_pairs results :" >> raport.txt
	./main 43 merge_files $(MEDIUM)
	
merge_few_big_pairs :
	echo "merge_few_big_pairs results :" >> raport.txt
	./main 3 merge_files $(BIG)
	
merge_more_big_pairs :
	echo "merge_more_big_pairs results :" >> raport.txt
	./main 11 merge_files $(BIG)
	
merge_many_big_pairs :
	echo "merge_many_big_pairs results :" >> raport.txt
	./main 43 merge_files $(BIG)
