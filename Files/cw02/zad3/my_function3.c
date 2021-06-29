#include "my_function3.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

void check_numbers_lib(){
 char *file_name = "dane.txt";
 FILE *f1;
 if ((f1 = fopen(file_name, "r")) == NULL){
  fprintf(stdout, "%s\n", "ERROR : Can not open file!");
  return;
 }

 FILE *a_file = fopen("a_lib.txt", "w+");
 FILE *b_file = fopen("b_lib.txt", "w+");
 FILE *c_file = fopen("c_lib.txt", "w+");
 
 
 size_t buffer_size = 0;
 char *buffer = NULL;
 
 int number;
 int even_counter = 0;
 int l,r;
 int middle_value, middle_value_sqr;
 
 while(getline(&buffer, &buffer_size, f1) != -1){
  number = atoi(buffer);
  if (!(number & 1)){ even_counter++; }
     
  if (number/10 > 0 && (((number%100)/10) == 0 || ((number%100)/10) == 7)){
   fputs(buffer, b_file);
  }
  
  l = 1;
  r = number;
  while (l < r){
   middle_value = (l + r) / 2;
   middle_value_sqr = middle_value * middle_value;
   if (middle_value_sqr == number){
    fputs(buffer, c_file);
    r = -1 ;// exit from while loop
   } else if (middle_value_sqr < number){
    l = middle_value + 1;
   } else{
    r = middle_value - 1;
   }
  }
 }
 fprintf(a_file , "Liczb parzystych jest %d\n", even_counter);
 
 free(buffer);
 fclose(a_file);
 fclose(b_file);
 fclose(c_file);
 fclose(f1);
}


void check_numbers_sys(){
 char *file_name = "dane.txt";
 int f;
 if ((f = open(file_name, O_RDONLY)) < 0){
  write(1, "Can not open file!\n", 19);
  return;
 }
 
 int a_file, b_file, c_file;
 if ((a_file=open("a_sys.txt",O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IXUSR))< 0){
  write(1, "Can not open/create the files!\n", 31);
  return;
 }
 
 if ((b_file=open("b_sys.txt",O_WRONLY | O_CREAT | O_TRUNC |  O_APPEND, S_IRUSR | S_IWUSR | S_IXUSR))< 0){
  write(1, "Can not open/create the files!\n", 31);
  return;
 }
 if ((c_file=open("c_sys.txt",O_WRONLY | O_CREAT | O_TRUNC | O_APPEND, S_IRUSR | S_IWUSR | S_IXUSR))< 0){
  write(1, "Can not open/create the files!\n", 31);
  return;
 }
 char buffer[256];
 int number;
 int even_counter = 0;
 int l,r;
 int middle_value, middle_value_sqr;
 char c;
 int i = 0;
 while (read(f, &c, 1) > 0){
  if (c != '\n'){
   buffer[i++] = c;
  } else{
   buffer[i++] = c;
   char *number_string = calloc(i, sizeof(char));
   strncpy(number_string, buffer , i);
   number = atoi(number_string);
   
   if (!(number & 1)) { even_counter++; }
   
   if ((number/10 > 0) && ((((number % 100) / 10) == 0) || ((number % 100) / 10) == 7)){
    write(b_file, buffer, i);
   }
   
   l = 1;
   r = number;
   while (l < r){
    middle_value = (l + r) / 2;
    middle_value_sqr = middle_value * middle_value;
    if (middle_value_sqr == number){
     write(c_file, number_string, i);
     r = -1 ;// exit from while loop
    } else if (middle_value_sqr < number){
     l = middle_value + 1;
    } else{
    r = middle_value - 1;
    }
   }
   free(number_string);
   i = 0;
  }
 }
 int even_counter_len = 1;
 int x = even_counter;
 while (x /= 10) { even_counter_len++;}
 
 char even_c[even_counter_len+1];
 sprintf(even_c, "%d", even_counter);
 char parzystych[] = "Liczb parzystych jest ";
 strcat(parzystych, even_c);
 write(a_file , parzystych , strlen(parzystych));
 
 
 close(a_file);
 close(b_file);
 close(c_file);
 close(f);
}
