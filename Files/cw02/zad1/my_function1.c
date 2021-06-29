#include "my_function1.h"
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

void print_alternately_lib(int argc, char *argv[]){
 char f_name1[256];
 char f_name2[256];
 
 FILE *f1;
 FILE *f2;

 if (argc < 5) {
  printf("%s\n", "Enter 2 file names!");
  scanf("%s %s", f_name1, f_name2);
  if ((f1 = fopen(f_name1, "r")) == NULL) { printf("%s\n", "Could not open the file!");}
  if ((f2 = fopen(f_name2, "r")) == NULL) { printf("%s\n", "Could not open the file!");}
 } else if (argc > 5){
  printf("%s\n", "Too many arguments!");
  return ;
 } else {
  if ((f1 = fopen(argv[3], "r")) == NULL) { printf("%s\n", "Could not open the file!");}
  if ((f2 = fopen(argv[4], "r")) == NULL) { printf("%s\n", "Could not open the file!");}
 }

 size_t buffer_size = 0;
 char *buffer1 = NULL;
 char *buffer2 = NULL;
 int read1, read2;
 while (((read1 = getline(&buffer1,&buffer_size,f1)) != -1)&&
 	((read2 = getline(&buffer2,&buffer_size,f2)) != -1)){
  fprintf(stdout, "%s", buffer1);
  fprintf(stdout, "%s", buffer2);
 }
 if (read1 != -1){
  fprintf(stdout, "%s", buffer1);
  while (getline(&buffer1, &buffer_size, f1) != -1){
  printf("%s", buffer1);
  }
 } else if (read2 != -1){
  fprintf(stdout, "%s", buffer2);
  while (getline(&buffer2, &buffer_size, f2) != -1){
  printf("%s", buffer2);
  }
 }
 
 free(buffer1);
 free(buffer2);
 if (fclose(f1) != 0) { fprintf(stdout, "%s\n", "The file could not be closed!");}
 if (fclose(f2) != 0) { fprintf(stdout, "%s\n", "The file could not be closed!");}
}


void print_alternately_sys(int argc, char * argv[]){
 char f_name1[256];
 char f_name2[256];
 
 int f1, f2;

 if (argc < 5) {
  printf("%s\n", "Enter 2 file names!");
  scanf("%s %s", f_name1, f_name2);
  if ((f1 = open(f_name1, O_RDONLY)) < 0) { printf("%s\n", "Could not open the file!");}
  if ((f2 = open(f_name2, O_RDONLY)) < 0) { printf("%s\n", "Could not open the file!");}
 } else if (argc > 5){
  printf("%s\n", "Too many arguments!");
  return ;
 } else {
  if ((f1 = open(argv[3], O_RDONLY)) < 0) { printf("%s\n", "Could not open the file!");}
  if ((f2 = open(argv[4], O_RDONLY)) < 0) { printf("%s\n", "Could not open the file!");}
 }

 
 char buffer1[256];
 char buffer2[256];
 int read1, read2;
 char c1, c2;
 
 int i = 0;
 int j = 0;
 while (((read1 = read(f1, &c1, 1)) > 0) && ((read2 = read(f2, &c2, 1)) > 0)){
  if (c1 == '\n'){
   buffer1[i++] = c1;
   write(1, buffer1, i); // print buffer1 to stdout
   buffer2[j++] = c2;
   if (c2 != '\n'){
    while((read(f2, &c2, 1 ) > 0) && (c2 != '\n')){
     buffer2[j++] = c2;
    }
    if (c2 == '\n') { buffer2[j++] = c2;}
   }
   write(1, buffer2, j); // print buffer2 to stdout
   i=0;
   j=0;
  } else if (c2 == '\n'){
   buffer2[j++] = c2;
   buffer1[i++] = c1;
   if (c1 != '\n'){
    while((read(f1, &c1, 1 ) > 0) && (c1 != '\n')){
     buffer1[i++] = c1;
    }
    if (c1 == '\n') { buffer1[i++] = c1;}
   }
   write(1, buffer1, i); // print buffer1 to stdout
   write(1, buffer2, j); // print buffer2 to stdout
   i=0;
   j=0;
  } else {
   buffer1[i++] = c1;
   buffer2[j++] = c2;
  }
 }
 
 
 if (read1 <= 0 && read2 <= 0){
  write(1, buffer1, i); // print buffer1 to stdout
  write(1, buffer2, j); // print buffer2 to stdout
 } else if (read1 > 0){
  buffer1[i++] = c1;
  int flag = 0;
  while (read(f1, &c1, 1) > 0){
   if (c1 == '\n'){
    buffer1[i++] = c1;
    write(1, buffer1, i); // print buffer1 to stdout
    if (flag == 0){
     write(1, buffer2, j); // print buffer2 to stdout
     flag = 1;
    }
    i = 0;
   } else{
    buffer1[i++] = c1;
   }
  }
 } else if (read2 > 0){
  
  write(1, buffer1, i); // print buffer1 to stdout
  buffer2[j++] = c2;
  
  while (read(f2, &c2, 1) > 0){
   if (c2 == '\n'){
    buffer2[j++] = c2;
    write(1, buffer2, j); // print buffer2 to stdout
    j = 0;
   } else{
    buffer2[j++] = c2;
   }
  }
 } 
 
 close(f1);
 close(f2);
}




