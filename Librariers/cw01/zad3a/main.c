#include <stdio.h>
#include <stdlib.h>
#include "my_library.h"
#include <string.h>
#include <time.h>
#include <sys/times.h>

int main(int argc, char* argv[]) {
 if (argc < 2){
  printf("%s\n", "There must be >= 2 arguments!");
  return 0;
 }
 
 
 
 struct tms tms_start;
 struct tms tms_end;
 clock_t start_time;
 
 char * option = argv[1];
 int main_table_elements;
 int opt_flag = 0;
 if (strcmp(option, "saving") == 0 || 
     strcmp(option, "removing") == 0 || 
     strcmp(option, "mix") == 0){
  main_table_elements = atoi(argv[2]);
  opt_flag = 1;
 } else{
  main_table_elements = atoi(argv[1]);
  start_time = times(&tms_start);
 }
 
 Main_table main_table = create_table(main_table_elements);

 int i;
 if (opt_flag == 1){
  i = 3;
 } else{
  i = 2;
 }
 while (i < argc){
  if (opt_flag == 1 && strcmp(option, "saving") == 0 && 
      i == 3 + main_table_elements){
   start_time = times(&tms_start);
  } else if (opt_flag == 1 &&
            (strcmp(option, "saving") == 0 || strcmp(option, "mix") == 0 || strcmp(option, "removing") == 0) && 
            i == 4 + main_table_elements){
   start_time = times(&tms_start);
  }
  
  char *command = argv[i++];
  if (strcmp(command, "create_table") == 0){
   for (int j = main_table.size - 1 ; j >= 0 ; j--){
    remove_block(&main_table,j);
   }
   main_table_elements = atoi(argv[i++]);
   main_table = create_table(main_table_elements);
  } else if (strcmp(command, "create_block") == 0){
   create_block(&main_table);
  } else if (strcmp(command, "merge_files") == 0){
   int k = 0;
   while ((i < argc) && (strchr(argv[i], ':') != NULL)){
    char *pair = argv[i++];
    char *token = strtok(pair, ":");
    char *sequence[2];
    for (int j = 0 ; j < 2 ; j++){
     sequence[j] = token;
     token = strtok(NULL, ":");
    }
    merge_files(k++, sequence);
   }
  } else if (strcmp(command, "remove_block") == 0){
   int idx = atoi(argv[i++]);
   remove_block(&main_table, idx);
  } else if (strcmp(command, "remove_row") == 0){
   int block_index = atoi(argv[i++]);
   int row_index = atoi(argv[i++]);
   Merged_file *merged_file = main_table.merged[block_index];
   remove_row(merged_file, row_index);
  } else if (strcmp(command, "print_merged") == 0){
   print_merged(&main_table);
  } else{
   printf("%s %s\n", "WARNING : Invalid command! = ", command);
  }
 }
 clock_t end_time = times(&tms_end);
 
 char raport[] = "results3a.txt";
 FILE *f = fopen(raport, "a");
 fprintf(f, "Real time : %0.10f\n",
        (double)(end_time - start_time)/ CLOCKS_PER_SEC );
 fprintf(f, "User time : %0.10f\n",
        (double)(tms_end.tms_utime - tms_start.tms_utime)/ CLOCKS_PER_SEC );
 fprintf(f, "Cpu time : %0.10f\n",
        (double)(tms_end.tms_stime - tms_start.tms_stime)/ CLOCKS_PER_SEC );
 fclose(f);
 
 
 
 return 0;
}

