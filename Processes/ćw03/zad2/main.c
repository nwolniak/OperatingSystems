#include <stdio.h>
#include <stdlib.h>
#include "my_library.h"
#include <string.h>
#include <time.h>
#include <sys/times.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


int main(int argc, char* argv[]) {
 if (argc != 4){
  printf("%s\n", "There must be  3 arguments!");
  return 0;
 }
 
 struct tms tms_start;
 struct tms tms_end;
 clock_t start_time, end_time;
 
 int n = atoi(argv[1]);
 Main_table main_table = create_table(n);

 start_time = times(&tms_start);
 char *command = argv[2];
 if (strcmp(command, "merge_files") == 0){
  pid_t pid;
  for (int i = 0 ; i < n ; i++){
   char *pair = argv[3];
   char *token = strtok(pair, ":");
   char *sequence[2];
   for (int j = 0 ; j < 2 ; j++){
    sequence[j] = token;
    token = strtok(NULL, ":");
   }
   if ((pid = fork()) == 0){
    merge_files(sequence);
    exit(0);
   }
  }
  for (int i = 0 ; i < n ; i++){
   if (wait(0) == -1)
    printf("Error in wait\n");
  }
 } else{
  printf("%s %s\n", "WARNING : Invalid command! = ", command);
 }
 end_time = times(&tms_end);
 
 for (int j = main_table.size - 1 ; j >= 0 ; j--){
  remove_block(&main_table,j);
 }
 
 FILE *f;
 if ((f = fopen("raport.txt", "a")) == NULL){
  printf("Could not create raport.txt file!\n");
  return 0;
 }
 double real_times = (double)((end_time - start_time));  
 double user_times = (double)((tms_end.tms_cutime - tms_start.tms_cutime));
 double cpu_times = (double)((tms_end.tms_cstime - tms_start.tms_cstime));
 user_times += (double)((tms_end.tms_utime - tms_start.tms_utime));
 cpu_times += (double)((tms_end.tms_stime - tms_start.tms_stime));
 
 fprintf(f, "Real time : %f\n", (double)(real_times));
 fprintf(f, "User time : %f\n", (double)(user_times));
 fprintf(f, "Cpu time : %f\n", (double)(cpu_times));
 fclose(f);
 
 return 0;
}

