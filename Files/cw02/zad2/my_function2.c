#include "my_function2.h"
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

void print_lines_with_lib(int argc, char *argv[]){
 if (argc < 5 || argc > 5){
  fprintf(stdout, "%s\n", "Invalid number of arguments!");
  return;
 }

 char *c_user = argv[3];
 FILE *f_in;
 if ((f_in = fopen(argv[4], "r")) == NULL){
  fprintf(stdout, "%s\n", "ERROR : Can not open file!");
  return ;
 }

 char buffer[256];
 int flag = 0;
 int i = 0;
 char c;

 while((c = fgetc(f_in)) != EOF){
  if ( c == *c_user){
   flag = 1;
  }
  
  if (c != '\n'){
   buffer[i++] = c;
  } else{
   if (flag == 1){
    buffer[i++] = c;
    fwrite(buffer, 1, i, stdout); // buffer[0:i]
   }
   flag = 0;
   i = 0;
  }
 }
 
 fclose(f_in);
}


void print_lines_with_sys(int argc, char *argv[]){
 if (argc < 5 || argc > 5){
  write(1, "Invalid number of arguments!\n", 29);
  return;
 }

 char *c_user = argv[3];
 int f_in;
 if ((f_in = open(argv[4], O_RDONLY)) < 0 ){
  write(1, "Can not open file!\n", 19);
  return ;
 }
 char buffer[256];
 int flag = 0;
 int i = 0;
 char c;
 while((read(f_in, &c, 1)) > 0){
  if ( c == *c_user){
   flag = 1;
  }
  
  if (c != '\n'){
   buffer[i++] = c;
  } else{
   if (flag == 1){
    buffer[i++] = c;
    write(1, buffer, i); // write buffer to stdout
   }
   flag = 0;
   i = 0;
  }
 }
 
 close(f_in);
}
