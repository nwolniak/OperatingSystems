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


void merge_files(int n, char **sequences) {
 char mergedABn[] = "mergedAB00.tmp";
 FILE *textA = fopen(*(sequences), "r");
 FILE *textB = fopen(*(sequences+1), "r");
  
 mergedABn[8] = ((n+1)/10) + '0';
 mergedABn[9] = ((n+1)%10) + '0';

  
 FILE *mergedAB = fopen(mergedABn, "w");
 char * lineA = NULL;
 char * lineB = NULL;
 size_t len = 0;
 while (((getline(&lineA, &len, textA)) != -1) && (getline(&lineB, &len, textB) != -1)) {
  fputs(lineA, mergedAB);
  fputs(lineB, mergedAB);
 }
 free(lineA);
 free(lineB);
 fclose(textA);
 fclose(textB);
 fclose(mergedAB);
 
}


int create_block(Main_table *main_table){
 int returning_idx = main_table->size;

 char mergedABn[] = "mergedAB__.tmp";
 mergedABn[8] = ((main_table->size +1)/10) + '0';
 mergedABn[9] = ((main_table->size +1)%10) + '0';
 FILE *merged = fopen(mergedABn, "r");

 while (merged != NULL){
  int line_counter = 0;
  size_t len = 0;
  char *line = NULL;
  while ((getline(&line, &len, merged)) != -1){
   line_counter++;
  }
  rewind(merged);

  char ** merged_file_lines = calloc(line_counter, sizeof(char*));
  line_counter = 0;
  while ((getline(&line, &len, merged)) != -1){
   merged_file_lines[line_counter] = line;
   line_counter++;
  }
  Merged_file *merged_file = calloc(1, sizeof(Merged_file));
  merged_file->lines = merged_file_lines;
  merged_file->size = line_counter;
  main_table->merged[main_table->size] = merged_file;
  main_table->size++;

  free(line);
  fclose(merged);

  mergedABn[8] = ((main_table->size + 1)/10) + '0';
  mergedABn[9] = ((main_table->size + 1)%10) + '0';
  merged = fopen(mergedABn, "r");
  }
 return returning_idx;
}


int lines_number(Merged_file *merged_file){
 return merged_file->size;
}


void remove_row(Merged_file *merged_file, int idx){
 if (idx < 0 || idx >= merged_file->size){
  printf("%s\n", "WARNING : Row invalid index !");
  return;
 }

 //char *row_deleted = merged_file->lines[idx];

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


void print_merged(Main_table *main_table){
 int size = main_table->size;
 for (int i = 0 ; i < size ; i++){
  int line_counter = main_table->merged[i]->size;
  for (int j = 0 ; j < line_counter ; j++){
   printf("%s\n", main_table->merged[i]->lines[j]);
  }
 }
}






