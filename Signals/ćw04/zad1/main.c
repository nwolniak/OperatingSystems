#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

void sig_handler(int signo){
	printf("Handled %s signal\n", strsignal(signo));
}


int main(int argc, char *argv[]){
	if (argc != 2){
		printf("Invalid number of arguments!\n");
		exit(1);
	}

	char* command = argv[1];
	int my_sig = SIGINT;
	pid_t child;
	if ((child = fork()) == -1){
		printf("Could not create child process!\n");
		exit(1);
	} else if (child == 0){
		/* Child */
		printf("In child process\n");
		if (strcmp(command, "ignore") == 0){

			if (signal(my_sig, SIG_IGN) == SIG_ERR){
				printf("%s can not be ignored!\n", strsignal(my_sig));
				exit(1);
			}


		} else if (strcmp(command, "handler") == 0){

			if (signal(my_sig, sig_handler) == SIG_ERR){
				printf("%s can not be handled!\n", strsignal(my_sig));
				exit(1);
			}

		} else if (strcmp(command, "mask") == 0 || strcmp(command, "pending") == 0){
			sigset_t new_mask;
			sigemptyset(&new_mask);
			sigaddset(&new_mask, my_sig);
			if (sigprocmask(my_sig, &new_mask, NULL) == -1){
				printf("Could not mask the signal!\n");
				exit(1);
			}
			printf("Masked %s signal\n", strsignal(my_sig));
		}

		/* raising signal in child process */
		printf("Raising %s in child process\n", strsignal(my_sig));
		raise(my_sig);

		if (strcmp(command, "pending") == 0){
			raise(my_sig);
			/* Pending signals set */
			sigset_t sig_pending;
			sigpending(&sig_pending);

			if (sigismember(&sig_pending, my_sig) == 1){
				printf("%s is pending in child1 process\n", strsignal(my_sig));
			} else{
				printf("%s is not pending in child1 process\n", strsignal(my_sig));
			}
		}

		pid_t next_child;
		if ((next_child = fork()) == -1){
			printf("Could not create child process!\n");
			exit(1);
		}
		if (next_child == 0){
			/* Next child*/
			printf("In next child process\n");
			if (strcmp(command, "ignore") == 0 ||
				strcmp(command, "handler") == 0 ||
				strcmp(command, "mask") == 0){
				/* raising signal in next child process */
				printf("Raising %s in next_child process\n", strsignal(my_sig));
				raise(my_sig);

			} else if (strcmp(command, "pending") == 0){
				/* Next child pending signals set*/
				sigset_t sig_pending2;
				sigpending(&sig_pending2);

				if (sigismember(&sig_pending2, my_sig) == 1){
					printf("%s is pending in child2 process\n", strsignal(my_sig));
				} else{
					printf("%s is not pending in child2 process\n", strsignal(my_sig));
				}
			}
			exit(0);

		} else{
			/* Child process */
			int status;
			waitpid(next_child, &status, WUNTRACED);
			if (WIFSIGNALED(status)){
				printf("Next child process terminated by a %s signal\n", strsignal(WTERMSIG(status)));
			} else if (WIFSTOPPED(status)){
				printf("Next child process stopped by a %s signal\n", strsignal(WSTOPSIG(status)));
			} else{
				printf("Next child exit status %d\n", status);
			}
			exit(0);
		}

	} else if (!(strcmp(command, "handler") == 0 )){
		/* Parent */
		/* wait for child1 process */
		wait(0);

		if ((child = fork()) == -1){
			printf("Could not create child process!\n");
			exit(1);
		} else if (child == 0){
			/* child2 process */
			printf("In child2 process\n");
			if (strcmp(command, "ignore") == 0){

				if (signal(my_sig, SIG_IGN) == SIG_ERR){
					printf("%s can not be ignored!\n", strsignal(my_sig));
					exit(1);
				}

			} else if (strcmp(command, "mask") == 0 || strcmp(command, "pending") == 0){

				sigset_t new_mask;
				sigemptyset(&new_mask);
				sigaddset(&new_mask, my_sig);
				if (sigprocmask(my_sig, &new_mask, NULL) == -1){
					printf("Could not mask the signal!\n");
					exit(1);
				}
				printf("Masked %s signal\n", strsignal(my_sig));
				
			}

			/* raising signal in child2 process */
			printf("Raising %s in child2 process\n", strsignal(my_sig));
			raise(my_sig);

			if (strcmp(command, "pending") == 0){
				/* Pending signals set */
				sigset_t sig_pending;
				sigpending(&sig_pending);

				if (sigismember(&sig_pending, my_sig) == 1){
					printf("%s is pending in child2 process\n", strsignal(my_sig));
				} else{
					printf("%s is not pending in child2 process\n", strsignal(my_sig));
				}
			}

			/* exec */
			printf("Exec next_child.c\n");
			char my_sig_str[12];
			sprintf(my_sig_str, "%d", my_sig);
			if (execl("/home/norbert/Desktop/NorbertWolniak/ćw04/zad1/next_child",
					  "next_child", command, my_sig_str, NULL) == -1){
				printf("Fail in execl\n");
				puts(strerror(errno));
				exit(1);
			}
		} else{
			/* Parent process */
			/* wait for child2 process */
			int status;
			waitpid(child, &status, WUNTRACED);
			if (WIFSIGNALED(status)){
				printf("Child2 process terminated by a %s signal\n", strsignal(WTERMSIG(status)));
			} else if (WIFSTOPPED(status)){
				printf("Child2 child process stopped by a %s signal\n", strsignal(WSTOPSIG(status)));
			} else{
				printf("Child2 exit status %d\n", status);
			}
		}
	} else{
		/* Parent */
		/* wait for child1 process if command is handler*/
		wait(0);
	}

	printf("Main program end\n");

	return 0;
}