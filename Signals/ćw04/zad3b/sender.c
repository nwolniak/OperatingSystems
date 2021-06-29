#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>


int call_back_signals = 0;
int flag = 1;
int catcher_signals = 0;

void handler_kill(int signo, siginfo_t *info, void* ucontext){
	printf("SENDER Handled %s signal\n", strsignal(signo));

	if (signo == SIGUSR1){
		call_back_signals ++;
	} else{
		flag = 0;
	}
}

void handler_sigqueue(int signo, siginfo_t *info, void* uncontext){
	if (signo == SIGUSR1){
		printf("SENDER Handled %s signal with number %d\n", strsignal(signo), info->si_value.sival_int);
		call_back_signals++;
		catcher_signals = info->si_value.sival_int;
	} else{
		printf("SENDER Handled %s signal\n", strsignal(signo));
		flag = 0;
	}
}

void handler_sigrt(int signo, siginfo_t *info, void * uncontext){
	printf("SENDER Handled %s signal\n", strsignal(signo));
	if (signo == SIGRTMIN){
		call_back_signals++;
	} else{
		flag = 0;
	}
}



int main(int argc, char* argv[]){
	if (argc != 4){
		printf("Invalid number of arguments!\n");
		return 1;
	}

	pid_t catcher = atoi(argv[1]);
	int n = atoi(argv[2]);
	char* mode = argv[3];

	/* SIGACTION */
	struct sigaction act;
	if (strcmp(mode, "kill") == 0){
		act.sa_sigaction = handler_kill;
	} else if (strcmp(mode, "sigqueue") == 0){
		act.sa_sigaction = handler_sigqueue;
	} else if (strcmp(mode, "sigrt") == 0){
		act.sa_sigaction = handler_sigrt;
	}
	sigfillset(&act.sa_mask);
	act.sa_flags = SA_RESTART | SA_SIGINFO;

	if (strcmp(mode, "sigrt") == 0){
		if (sigaction(SIGRTMIN, &act, NULL) == -1){
			printf("Error in sigaction()\n");
			puts(strerror(errno));
		}

		if (sigaction((SIGRTMIN + 1), &act, NULL) == -1){
			printf("Error in sigaction()\n");
			puts(strerror(errno));
		}
	} else{
		if (sigaction(SIGUSR1, &act, NULL) == -1){
			printf("Error in sigaction()\n");
			puts(strerror(errno));
		}

		if (sigaction(SIGUSR2, &act, NULL) == -1){
			printf("Error in sigaction()\n");
			puts(strerror(errno));
		}
	}

	/* MASK */
	sigset_t mask;
	sigfillset(&mask);
	if (strcmp(mode, "sigrt") == 0){
		sigdelset(&mask, SIGRTMIN);
		sigdelset(&mask, (SIGRTMIN + 1));
	} else{
		sigdelset(&mask, SIGUSR1);
		sigdelset(&mask, SIGUSR2);
	}

	for (int i = 0 ; i < n ; i ++){
		if (strcmp(mode, "kill") == 0){
			printf("Wysyłam SIGUSR1\n");
			kill(catcher, SIGUSR1);
		} else if (strcmp(mode, "sigqueue") == 0){
			union sigval value;
			value.sival_int = i;
			printf("Wysyłam SIGUSR1\n");
			sigqueue(catcher, SIGUSR1, value);
		} else if (strcmp(mode, "sigrt") == 0){
			printf("Wysyłam SIGRTMIN\n");
			kill(catcher, SIGRTMIN);
		}
		sigsuspend(&mask);
	}

	if (strcmp(mode, "kill") == 0){
		printf("Wysyłam SIGUSR2\n");
		kill(catcher, SIGUSR2);
	} else if (strcmp(mode, "sigqueue") == 0){
		union sigval value;
		value.sival_int = 0;
		printf("Wysyłam SIGUSR2\n");
		sigqueue(catcher, SIGUSR2, value);
	} else if (strcmp(mode, "sigrt") == 0){
		printf("Wysyłam SIGRTMIN + 1\n");
		kill(catcher, (SIGRTMIN + 1));
	}


	

	while(flag){
		sigsuspend(&mask);
	}


	if (strcmp(mode, "sigqueue") == 0){
		printf("Sender wysłał %d, odebrał %d , Catcher odebrał %d\n",n,call_back_signals, catcher_signals);
	}else{
		printf("Sender wysłał %d, odebrał %d\n",n, call_back_signals);
	}

	return 0;
}