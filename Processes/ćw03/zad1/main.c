#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>


int main(int argc, char *argv[]){
 int n;
 if (argc == 2){
  n = atoi(argv[1]);
 } else{
  printf("%s\n", "Invalid number of arguments!");
  return 1;
 }
 
 pid_t pid;
 printf("PID Procesu macierzystego PID = %d\n", (int)getpid());
 for (int i = 0 ; i < n ; i ++){
  if ((pid = fork()) == 0){
   printf("Komunikat z procesu o PID = %d\n", (int)getpid());
   exit(0);
  }
 }
 for (int i = 0 ; i < n ; i++){
    if (wait(0) == -1)
      printf("Error in wait\n");
 }
 return 0;
}
