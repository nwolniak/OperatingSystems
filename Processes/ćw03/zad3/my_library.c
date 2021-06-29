#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int find_pattern(char *path, char *pattern){
 FILE *f;
 if ((f = fopen(path, "r")) == NULL){
  printf("Could not open file %s\n", path);
  return 0;
 }
 
 char *buffer = NULL;
 size_t size_of_buffer = 0;
 
 while (getline(&buffer, &size_of_buffer, f) != -1){
  if (strstr(buffer, pattern) != NULL){
   free(buffer);
   return 1;
  }
 }
 free(buffer);
 return 0;
}
 
 
char *update_path(char *path, char *dir){
 if (strlen(path) == 0){
  char *new_path = calloc(strlen(dir) + 1, sizeof(char));
  sprintf(new_path, "%s",dir);
  return new_path;
 }
 char* new_path = calloc(strlen(path) + strlen(dir) + 2, sizeof(char));
 sprintf(new_path, "%s/%s", path, dir);
 return new_path;
}
