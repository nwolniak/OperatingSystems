CC=gcc
CFLAGS=-c -Wall
LIBRARY_NAME=my_library

static :
	$(CC) $(CFLAGS) my_library.c
	ar rcs libmy_library.a my_library.o
	$(CC) $(CFLAGS) main.c
	$(CC) main.c -L. -l my_library -o main
shared :
	$(CC) -fPIC $(CFLAGS) -shared $(LIBRARY_NAME).c -o $(LIBRARY_NAME).so
	$(CC) $(CFLAGS) main.c -L. -l$(LIBRARY_NAME) -o main
	
clean_tmp :
	rm -f *.tmp *.o *a *.so main

clean :
	rm -f *.o *a *.so *.tmp main raport2.txt

merge_few_small_pairs :
	make clean_tmp
	echo "merge_few_small_pairs results :" >> raport2.txt
	./main 3 merge_files smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt

merge_more_small_pairs :
	make clean_tmp
	echo "merge_more_small_pairs results :" >> raport2.txt
	./main 11 merge_files smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt \
	smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt \
	smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt
	
merge_many_small_pairs :
	make clean_tmp
	echo "merge_many_small_pairs results :" >> raport2.txt
	./main 43 merge_files smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt \
	smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt \
	smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt \
	smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt \
	smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt \
	smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt \
	smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt \
	smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt \
	smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt \
	smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt \
	smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt
	
	
merge_few_medium_pairs :
	make clean_tmp
	echo "merge_few_medium_pairs results :" >> raport2.txt
	./main 3 merge_files mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt
	

merge_more_medium_pairs:
	make clean_tmp
	echo "merge_more_medium_pairs results :" >> raport2.txt
	./main 11 merge_files mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt \
	mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt \
	mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt

merge_many_medium_pairs :
	make clean_tmp
	echo "merge_many_medium_pairs results :" >> raport2.txt
	./main 43 merge_files mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt \
	mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt \
	mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt \
	mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt \
	mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt \
	mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt \
	mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt \
	mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt \
	mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt \
	mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt \
	mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt



merge_few_big_pairs :
	make clean_tmp
	echo "merge_few_big_pairs results :" >> raport2.txt
	./main 3 merge_files bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt

	
merge_more_big_pairs :
	make clean_tmp
	echo "merge_more_big_pairs results :" >> raport2.txt
	./main 11 merge_files bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt \
	bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt \
	bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt

merge_many_big_pairs :
	make clean_tmp
	echo "merge_many_big_pairs results :" >> raport2.txt
	./main 43 merge_files bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt \
	bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt \
	bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt \
	bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt \
	bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt \
	bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt \
	bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt \
	bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt \
	bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt \
	bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt \
	bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt
	

save_few_small_pairs :
	make clean_tmp
	echo "save_few_small_pairs results :" >> raport2.txt
	./main saving 3 merge_files smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt create_block

save_more_small_pairs :
	make clean_tmp
	echo "save_more_small_pairs results :" >> raport2.txt
	./main saving 11 merge_files smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt \
	smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt \
	smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt create_block
	
save_many_small_pairs :
	make clean_tmp
	echo "save_many_small_pairs results :" >> raport2.txt
	./main saving 43 merge_files smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt \
	smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt \
	smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt \
	smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt \
	smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt \
	smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt \
	smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt \
	smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt \
	smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt \
	smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt \
	smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt create_block
	
	
save_few_medium_pairs :
	make clean_tmp
	echo "save_few_medium_pairs results :" >> raport2.txt
	./main saving 3 merge_files mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt create_block
	

save_more_medium_pairs:
	make clean_tmp
	echo "save_more_medium_pairs results :" >> raport2.txt
	./main saving 11 merge_files mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt \
	mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt \
	mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt create_block

save_many_medium_pairs :
	make clean_tmp
	echo "save_many_medium_pairs results :" >> raport2.txt
	./main saving 43 merge_files mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt \
	mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt \
	mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt \
	mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt \
	mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt \
	mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt \
	mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt \
	mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt \
	mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt \
	mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt \
	mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt create_block



save_few_big_pairs :
	make clean_tmp
	echo "save_few_big_pairs results :" >> raport2.txt
	./main saving 3 merge_files bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt create_block

	
save_more_big_pairs :
	make clean_tmp
	echo "save_more_big_pairs results :" >> raport2.txt
	./main saving 11 merge_files bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt \
	bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt \
	bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt create_block

save_many_big_pairs :
	make clean_tmp
	echo "save_many_big_pairs results :" >> raport2.txt
	./main saving 43 merge_files bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt \
	bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt \
	bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt \
	bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt \
	bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt \
	bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt \
	bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt \
	bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt \
	bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt \
	bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt \
	bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt create_block
	

remove_few_small_pairs :
	make clean_tmp
	echo "remove_few_small_pairs results :" >> raport2.txt
	./main removing 3 merge_files smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt create_block \
	remove_block 0 remove_block 0 remove_block 0

remove_more_small_pairs :
	make clean_tmp
	echo "remove_more_small_pairs results :" >> raport2.txt
	./main removing 11 merge_files smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt \
	smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt \
	smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt create_block \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0
	
remove_many_small_pairs :
	make clean_tmp
	echo "remove_many_small_pairs results :" >> raport2.txt
	./main removing 43 merge_files smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt \
	smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt \
	smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt \
	smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt \
	smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt \
	smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt \
	smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt \
	smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt \
	smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt \
	smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt \
	smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt create_block \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0
	
	
	
remove_few_medium_pairs :
	make clean_tmp
	echo "remove_few_medium_pairs results :" >> raport2.txt
	./main removing 3 merge_files mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt create_block \
	remove_block 0 remove_block 0 remove_block 0
	

remove_more_medium_pairs:
	make clean_tmp
	echo "remove_more_medium_pairs results :" >> raport2.txt
	./main removing 11 merge_files mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt \
	mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt \
	mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt create_block \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0

remove_many_medium_pairs :
	make clean_tmp
	echo "remove_many_medium_pairs results :" >> raport2.txt
	./main removing 43 merge_files mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt \
	mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt \
	mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt \
	mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt \
	mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt \
	mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt \
	mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt \
	mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt \
	mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt \
	mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt \
	mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt create_block \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0



remove_few_big_pairs :
	make clean_tmp
	echo "remove_few_big_pairs results :" >> raport2.txt
	./main removing 3 merge_files bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt create_block \
	remove_block 0 remove_block 0 remove_block 0

	
remove_more_big_pairs :
	make clean_tmp
	echo "remove_more_big_pairs results :" >> raport2.txt
	./main removing 11 merge_files bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt \
	bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt \
	bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt create_block \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0


remove_many_big_pairs :
	make clean_tmp
	echo "remove_many_big_pairs results :" >> raport2.txt
	./main removing 43 merge_files bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt \
	bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt \
	bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt \
	bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt \
	bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt \
	bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt \
	bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt \
	bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt \
	bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt \
	bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt \
	bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt create_block \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0
	


mix_few_small_pairs :
	make clean_tmp
	echo "mix_few_small_pairs results :" >> raport2.txt
	./main mix 3 merge_files smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt create_block \
	remove_block 0 remove_block 0 remove_block 0 \
	create_block \
	remove_block 0 remove_block 0 remove_block 0 \
	create_block \
	remove_block 0 remove_block 0 remove_block 0
	

mix_more_small_pairs :
	make clean_tmp
	echo "mix_more_small_pairs results :" >> raport2.txt
	./main mix 11 merge_files smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt \
	smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt \
	smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt create_block \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	create_block \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	create_block \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0
	
mix_many_small_pairs :
	make clean_tmp
	echo "mix_many_small_pairs results :" >> raport2.txt
	./main mix 43 merge_files smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt \
	smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt \
	smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt \
	smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt \
	smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt \
	smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt \
	smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt \
	smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt \
	smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt \
	smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt \
	smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt smallA.txt:smallB.txt create_block \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 \
	create_block \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 \
	create_block \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0
	
	
	
	
mix_few_medium_pairs :
	make clean_tmp
	echo "mix_few_medium_pairs results :" >> raport2.txt
	./main mix 3 merge_files mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt create_block \
	remove_block 0 remove_block 0 remove_block 0 \
	create_block \
	remove_block 0 remove_block 0 remove_block 0 \
	create_block \
	remove_block 0 remove_block 0 remove_block 0
	

mix_more_medium_pairs:
	make clean_tmp
	echo "mix_more_medium_pairs results :" >> raport2.txt
	./main mix 11 merge_files mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt \
	mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt \
	mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt create_block \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	create_block \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	create_block\
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0
	

mix_many_medium_pairs :
	make clean_tmp
	echo "mix_many_medium_pairs results :" >> raport2.txt
	./main mix 43 merge_files mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt \
	mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt \
	mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt \
	mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt \
	mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt \
	mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt \
	mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt \
	mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt \
	mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt \
	mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt \
	mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt mediumA.txt:mediumB.txt create_block \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 \
	create_block \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 \
	create_block \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0



mix_few_big_pairs :
	make clean_tmp
	echo "mix_few_big_pairs results :" >> raport2.txt
	./main mix 3 merge_files bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt create_block \
	remove_block 0 remove_block 0 remove_block 0 \
	create_block \
	remove_block 0 remove_block 0 remove_block 0 \
	create_block \
	remove_block 0 remove_block 0 remove_block 0
	

	
mix_more_big_pairs :
	make clean_tmp
	echo "mix_more_big_pairs results :" >> raport2.txt
	./main mix 11 merge_files bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt \
	bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt \
	bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt create_block \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	create_block \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	create_block \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0


mix_many_big_pairs :
	make clean_tmp
	echo "mix_many_big_pairs results :" >> raport2.txt
	./main mix 43 merge_files bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt \
	bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt \
	bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt \
	bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt \
	bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt \
	bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt \
	bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt \
	bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt \
	bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt \
	bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt \
	bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt bigA.txt:bigB.txt create_block \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 \
	create_block \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 \
	create_block \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 \
	remove_block 0
	
	
tests :
	make clean
	make static
	echo "Test : " >> raport2.txt
	make merge_few_small_pairs
	make merge_more_small_pairs
	make merge_many_small_pairs
	make merge_few_medium_pairs
	make merge_more_medium_pairs
	make merge_many_medium_pairs
	make merge_few_big_pairs
	make merge_more_big_pairs
	make merge_many_big_pairs
	
	make save_few_small_pairs
	make save_more_small_pairs
	make save_many_small_pairs
	make save_few_medium_pairs
	make save_more_medium_pairs
	make save_many_medium_pairs
	make save_few_big_pairs
	make save_more_big_pairs
	make save_many_big_pairs
	
	make remove_few_small_pairs
	make remove_more_small_pairs
	make remove_many_small_pairs
	make remove_few_medium_pairs
	make remove_more_medium_pairs
	make remove_many_medium_pairs
	make remove_few_big_pairs
	make remove_more_big_pairs
	make remove_many_big_pairs
	
	make mix_few_small_pairs
	make mix_more_small_pairs
	make mix_many_small_pairs
	make mix_few_medium_pairs
	make mix_more_medium_pairs
	make mix_many_medium_pairs
	make mix_few_big_pairs
	make mix_more_big_pairs
	make mix_many_big_pairs








































	
	
