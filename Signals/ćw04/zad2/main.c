#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

void handler3(int signo, siginfo_t *info, void* ucontext){
	printf("Handled %s signal\n", strsignal(signo));
	printf("Errno value %d\n", info->si_errno);
	printf("Signal code %d\n", info->si_code);
	printf("Sending signal user UID %d\n", (int)info->si_uid);

	printf("Handled signal with number %d\n", info->si_signo);
	printf("Sending signal process PID %d\n", (int)info->si_pid);
}

void handler(int signo){
	printf("Handled %s signal\n", strsignal(signo));
}


int main(){
	printf("Main PID %d\n", (int)getpid());
	int my_sig = SIGINT;
	pid_t child;
	if ((child = fork()) == -1){
		printf("Error in fork()\n");
		puts(strerror(errno));
	} else if (child == 0){
		/* Child process */
		printf("Child process PID %d\n", (int)getpid());
		struct sigaction act;
		act.sa_sigaction = handler3;
		sigfillset(&act.sa_mask);
		act.sa_flags = SA_SIGINFO;

		if (sigaction(my_sig, &act, NULL) == -1){
			printf("Error in sigaction()\n");
			puts(strerror(errno));
		}
		/* first signal */
		raise(my_sig);

		printf("----------------\n");

		struct sigaction act2;
		act2.sa_handler = handler;
		sigfillset(&act2.sa_mask);
		act2.sa_flags = SA_NOCLDSTOP | SA_RESTART;
		//act2.sa_flags = 0;
		if (sigaction(SIGCHLD , &act2, NULL) == -1){
			printf("Error in sigaction()\n");
			puts(strerror(errno));
		}
		/* second signal */
		pid_t test_child;
		if ((test_child = fork()) == -1){
			printf("Error in fork()\n");
			puts(strerror(errno));
		} else if (test_child == 0){
			sleep(2);
			exit(0);
		} else{
			sleep(1);
			printf("Stop test_child\n");
			kill(test_child, SIGSTOP);
			sleep(1);
			printf("Continue test_child\n");
			kill(test_child, SIGCONT);
			wait(0);
		}

		printf("----------------\n");

		struct sigaction act3;
		act3.sa_handler = handler;
		sigfillset(&act3.sa_mask);
		act3.sa_flags = SA_RESTART;
		if (sigaction(SIGINT, &act3, NULL) == -1){
			printf("Error in sigaction()\n");
			puts(strerror(errno));
		}

		if ((test_child = fork()) == -1){
			printf("Error in fork()\n");
			puts(strerror(errno));
		} else if (test_child == 0){
			sleep(1);
			kill(getppid(), SIGINT);
			exit(0);
		} else{
			printf("Input number :");
			int x = 0;
			scanf("%d", &x);
			printf("Number is %d \n", x);
			wait(0);
		}

		exit(0);
	} else{
		/* Parent process */
		wait(0);
	}

	return 0;
}