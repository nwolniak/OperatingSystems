#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <time.h>
#include <sys/times.h>

#include "my_function5.h"

int main(int argc, char *argv[]){
 struct tms tms_start;
 struct tms tms_end;
 clock_t start_time;
 clock_t end_time;
 char *command = argv[1];
 int n = atoi(argv[2]);
 double real_times = 0;
 double user_times = 0;
 double cpu_times = 0;
 
 if (strcmp(command, "test") == 0){
  char pomiar[] = "pomiar_zad_5.txt";
  FILE *f = fopen(pomiar, "w");
  start_time = times(&tms_start);
  for (int i = 0 ; i < n ; i++){ 
   break_lines_lib(argv[3], argv[4]);
  }
  end_time = times(&tms_end);
  real_times = (double)((end_time - start_time));  
  user_times = (double)((tms_end.tms_utime - tms_start.tms_utime));
  cpu_times = (double)((tms_end.tms_stime - tms_start.tms_stime));   
  fprintf(f, "%s Real time : %f\n","Lib ", (real_times/n));
  fprintf(f, "%s User time : %f\n","Lib ", (user_times/n));
  fprintf(f, "%s Cpu time : %f\n","Lib ", (cpu_times/n));
  
  real_times = 0;
  user_times = 0;
  cpu_times = 0;
  
  start_time = times(&tms_start);
  for (int i = 0 ; i < n ; i++){
   break_lines_sys(argv[3], argv[4]);
  }
  end_time = times(&tms_end);
  real_times = (double)((end_time - start_time));  
  user_times = (double)((tms_end.tms_utime - tms_start.tms_utime));
  cpu_times = (double)((tms_end.tms_stime - tms_start.tms_stime)); 
  fprintf(f, "%s Real time : %f\n","Sys ", (real_times/n));
  fprintf(f, "%s User time : %f\n","Sys ", (user_times/n));
  fprintf(f, "%s Cpu time : %f\n","Sys ", (cpu_times/n));
  
  fclose(f);
  } else if (strcmp(command, "lib") == 0){
   break_lines_lib(argv[3], argv[4]);
  } else if (strcmp(command, "sys") == 0){
   break_lines_sys(argv[3], argv[4]);
  }
  
 return 0;
}


