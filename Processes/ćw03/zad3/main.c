#include <stdio.h>
#include <stdlib.h>
#include "my_library.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>


int main(int argc, char *argv[]){
 if (argc != 4){
  printf("Invalid number of arguments!\n");
  return 1;
 }
 struct dirent *entry;
 DIR *curr_dir;
 char *curr_path = argv[1];
 char *r_path = "";
 if ((curr_dir = opendir(curr_path)) == NULL){
  printf("Could not open dir : %s\n",curr_path);
  return 1;
 }
 
 char *pattern = argv[2];
 int max_depth = atoi(argv[3]);
 int file_types_number = 1;
 const char *file_types[] = {".txt"};
 
 pid_t pid;
 int curr_depth = 0;
 if ((pid = fork()) == 0){
  while((entry = readdir(curr_dir)) != NULL && curr_depth <= max_depth){
   int type = entry->d_type;
   char *file_name = entry->d_name;
   if (strcmp(file_name,".") == 0) { continue; }
   if (strcmp(file_name,"..") == 0) { continue; }
   if (type == DT_DIR){
   
    if ((pid = fork()) == 0 ){
     curr_path = update_path(curr_path, file_name);
     r_path = update_path(r_path, file_name);
     curr_depth++;
     
     if ((curr_dir = opendir(curr_path)) == NULL){
      printf("Could not open dir : [%s] path %s\n", file_name, curr_path);
      exit(1);
     }
     
    } 
   } else if (type == DT_REG && strlen(file_name) > 3){
    int file_type_flag = 0;
    char *file_type = calloc(4, sizeof(char));
    int k = 0;
    for (int j=strlen(file_name)-4;j<strlen(file_name);j++) {file_type[k++]=file_name[j];}
    for (int j=0;j<file_types_number;j++) {
     if (strcmp(file_type, file_types[j]) == 0){
      file_type_flag = 1;
      break;
     }
    }
    if (file_type_flag == 1){
     char *file_path = update_path(curr_path, file_name);
     
     if (find_pattern(file_path, pattern) == 1){
      printf("%s %d\n", update_path(r_path, file_name), (int)getpid());
     }
     
     free(file_path);
     file_type_flag = 0;  
    }
    free(file_type);
   }
  }
  wait(NULL); // wait for all children of the process
  closedir(curr_dir);
  free(r_path);
  exit(0);
 }
 int status;
 waitpid(pid, &status, 0);
 if (status == 1){
  printf("Error in waitpid\n");
 }
 
 closedir (curr_dir);
 return 0;
}
