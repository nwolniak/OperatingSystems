#include "my_function5.h"
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

void break_lines_lib(char *f_in_name, char *f_out_name){
 FILE *f_in;
 if ((f_in = fopen(f_in_name,"r")) == NULL){
  printf("%s\n", "Could not open the file!");
  return;
 }
 
 FILE *f_out;
 if ((f_out = fopen(f_out_name, "w")) == NULL){
  printf("%s\n", "Could not open the file!");
  return;
 }
 
 
 int c;
 char *buffer = calloc(51, sizeof(char)); // 50 char + '\n'
 int i = 0;
 while ( (c = fgetc(f_in)) != EOF){
  if ((c == '\n') || (i == 50)){
   buffer[i] = '\n';
   fputs(buffer, f_out);
   free(buffer);
   buffer = (char*)calloc(51, sizeof(char));
   i = 0;
   if (c != '\n'){
    buffer[i++] = c;
   }
  } else{
   buffer[i++] = c;
  }
 }
 
 free(buffer);
 fclose(f_in);
 fclose(f_out);
}


void break_lines_sys(char *f_in_name, char *f_out_name){
 int f_in;
 if ((f_in = open(f_in_name, O_RDONLY)) < 0){
  write(1, "Could not open the file!\n", 19);
  return;
 }
 
 int f_out;
 if ((f_out = open(f_out_name, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IXUSR )) < 0){
  write(1, "Could not open the file!\n", 19);
  return;
 }
 
 char c;
 char *buffer = calloc(51, sizeof(char)); // 50 char + '\n'
 int i = 0;
 while (read(f_in, &c, 1) > 0){
  if ((c == '\n') || (i == 50)){
   buffer[i++] = '\n';
   write(f_out, buffer, i);
   free(buffer);
   buffer = calloc(51, sizeof(char));
   i = 0;
   if (c != '\n'){
    buffer[i++] = c;
   }
  } else{
   buffer[i++] = c;
  }
 }
 
 free(buffer);
 close(f_in);
 close(f_out);
}

