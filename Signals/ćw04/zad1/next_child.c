#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>



int main(int argc, char* argv[]){
	printf("In next_child2 program\n");

	if (argc != 3){
		printf("Invalid number of arguments!\n");
		return 1;
	}

	char* command = argv[1];
	int my_sig = atoi(argv[2]);

	printf("Raising %s in next_child2 program\n", strsignal(my_sig));
	raise(my_sig);

	if (strcmp(command, "pending") == 0){
		/* Pending signals set */
		sigset_t sig_pending;
		sigpending(&sig_pending);

		if (sigismember(&sig_pending, my_sig) == 1){
			printf("%s is pending in next_child program\n", strsignal(my_sig));
		} else{
			printf("%s is not pending in next_child program\n", strsignal(my_sig));
		}
	}


	return 0;
}