#include "my_library.h"
#include <stdio.h>
#include <stdlib.h>


Main_table create_table(int size) {
    Merged_file **merged = calloc(size, sizeof(Merged_file*));
    for (int i = 0; i < size; i++) {
        merged[i] = NULL;
    }
    Main_table main_table;
    main_table.merged = merged;
    main_table.size = 0;
    return main_table;
}


void merge_files(char **sequences) {
 FILE *textA = fopen(*(sequences), "r");
 FILE *textB = fopen(*(sequences+1), "r");
  
 FILE *tmp;
 if ((tmp = tmpfile()) == NULL ){
  printf("Could not create tmp file!\n");
 }
 
 char * lineA = NULL;
 char * lineB = NULL;
 size_t len = 0;
 while (((getline(&lineA, &len, textA)) != -1) && (getline(&lineB, &len, textB) != -1)) {
  fputs(lineA, tmp);
  fputs(lineB, tmp);
 }
 free(lineA);
 free(lineB);
 fclose(textA);
 fclose(textB);
 fclose(tmp);
}


void remove_row(Merged_file *merged_file, int idx){
 if (idx < 0 || idx >= merged_file->size){
  printf("%s\n", "WARNING : Row invalid index !");
  return;
 }

 for (int i = idx ; i < merged_file->size -1 ; i++){
  merged_file->lines[i] = merged_file->lines[i+1];
 }
 merged_file->size = merged_file->size -1;
 merged_file->lines = realloc(merged_file->lines, merged_file->size * sizeof(char *));
}


void remove_block(Main_table *main_table, int idx){
 if (idx < 0 || idx >= main_table->size){
  printf("%s\n", "WARNING : Block invalid index !");
  return;
 }

 Merged_file *merged_file_deleted = main_table->merged[idx];

 for (int i = idx ; i < main_table->size -1 ; i++){
  main_table->merged[i] = main_table->merged[i+1];

  if (i+1 == main_table->size -1){
   main_table->merged[i+1] = NULL;
  }
 }

 int line_counter = merged_file_deleted->size;
 for (int i = line_counter - 1 ; i >= 0 ; i--){
  remove_row(merged_file_deleted, i);
 }

 main_table->size = main_table->size - 1;
 free(merged_file_deleted);
}

