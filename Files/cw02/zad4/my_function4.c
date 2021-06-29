#include "my_function4.h"
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

void change_lines_lib(char *f_in_name, char *f_out_name, char *n1, char *n2){
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
 int n1_size = 0;
 while(*(n1 + n1_size++) != '\0');
 if (n1_size-- < 1){
  printf("The string length is too small !");
 }
 
 char *fail_buffer = calloc(n1_size, sizeof(char));
 while ( (c = fgetc(f_in)) != EOF){
  if (c == n1[0]){
   for (int i = 0 ; i < n1_size ; i++){
    if ((c == EOF) || (c == '\n') || (c != n1[i])){
     fail_buffer[i] = c;
     fputs(fail_buffer, f_out);
     break;
    } else if (i == n1_size - 1){
     fputs(n2, f_out);
    } else{
     fail_buffer[i] = c;
     c = fgetc(f_in);
    }
   }
   free(fail_buffer);
   fail_buffer = calloc(n1_size, sizeof(char));
  } else{
   fputc(c, f_out);
  }
 }
 free(fail_buffer);
 fclose(f_in);
 fclose(f_out);
}



void change_lines_sys(char *f_in_name, char *f_out_name, char *n1, char *n2){
 int f_in;
 if ((f_in = open(f_in_name, O_RDONLY)) < 0){
  write(1, "Could not open the file!\n", 19);
  return;
 }
 
 int f_out;
 if ((f_out = open(f_out_name, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IXUSR))<0){
  write(1, "Could not open the file!\n", 19);
  return;
 }
 
 char c;
 int n1_size = 0;
 while(*(n1 + n1_size++) != '\0');
 if (n1_size-- < 1){
  write(1, "The string length is too small!\n", 32);
 }
 int n2_size = 0;
 while(*(n2 + n2_size++) != '\0');
 n2_size--;
 
 char *fail_buffer = calloc(n1_size, sizeof(char));
 
 while (read(f_in, &c, 1) > 0){
  if (c == n1[0]){
   fail_buffer[0] = c;
   for (int i = 1 ; i < n1_size ; i++){
    if (read(f_in, &c, 1) <= 0){
     write(f_out, fail_buffer, i-1);
     break;
    } else if ((c == '\n') || c != n1[i]){
     fail_buffer[i] = c;
     write(f_out, fail_buffer, i+1);
     break;
    } else if (i == n1_size -1){
     write(f_out, n2, n2_size);
    } else{
     fail_buffer[i] = c;
    }
   }
   free(fail_buffer);
   fail_buffer = calloc(n1_size, sizeof(char));
  } else{
   write(f_out, &c, 1);
  }
 }
 
 free(fail_buffer);
 close(f_in);
 close(f_out);
}

