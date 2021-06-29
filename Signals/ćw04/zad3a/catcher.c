#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

int signals = 0;


void handler_kill(int signo, siginfo_t *info, void* ucontext){
	printf("CATCHER Handled %s signal\n", strsignal(signo));

	if (signo == SIGUSR1){
		signals++;
	} else{
		for (int i = 0 ; i < signals ; i ++){
			printf("Wysyłam SIGUSR1 CATCHER\n");
			kill(info->si_pid, SIGUSR1);
		}
		printf("Wysyłam SIGUSR2 CATCHER\n");
		kill(info->si_pid, SIGUSR2);
		exit(0);
	}

}

void handler_siqqueue(int signo, siginfo_t *info, void* uncontext){
	if (signo == SIGUSR1){
		printf("CATCHER Handled %s signal with number %d\n", strsignal(signo), info->si_value.sival_int);
		signals++;
	} else{
		printf("CATCHER Handled %s signal\n", strsignal(signo));
		for (int i = 0 ; i < signals ; i ++){
			printf("Wysyłam SIGUSR1 CATCHER\n");
			union sigval value;
			value.sival_int = signals;
			sigqueue(info->si_pid, SIGUSR1, value);
		}
		printf("Wysyłam SIGUSR2 CATCHER\n");
		kill(info->si_pid, SIGUSR2);
		exit(0);
	}
}

void handler_sigrt(int signo, siginfo_t * info, void* uncontext){
	printf("CATCHER Handled %s signal\n", strsignal(signo));

	if (signo == SIGRTMIN){
		signals++;
	} else{
		for (int i = 0 ; i < signals ; i ++){
			printf("Wysyłam SIGRTMIN CATCHER\n");
			kill(info->si_pid, SIGRTMIN);
		}
		printf("Wysyłam SIGRTMIN + 1 CATCHER\n");
		kill(info->si_pid, (SIGRTMIN + 1));
		exit(0);
	}
}


int main(int argc, char* argv[]){
	if (argc != 2){
		printf("Invalid number of arguments!\n");
		return 1;
	}

	printf("Catcher PID %d\n", (int)getpid());

	char* mode = argv[1];

	/* SIGACTION */
	struct sigaction act;
	if (strcmp(mode, "kill") == 0){
		act.sa_sigaction = handler_kill;
	} else if (strcmp(mode, "sigqueue") == 0){
		act.sa_sigaction = handler_siqqueue;
	} else if (strcmp(mode, "sigrt") == 0){
		act.sa_sigaction = handler_sigrt;
	} else{
		printf("Invalid input\n");
		return 1;
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

	/* SIGSUSPEND */
	sigset_t mask;
	sigfillset(&mask);
	if (strcmp(mode, "sigrt") == 0){
		sigdelset(&mask, SIGRTMIN);
		sigdelset(&mask, (SIGRTMIN + 1));
	} else{
		sigdelset(&mask, SIGUSR1);
		sigdelset(&mask, SIGUSR2);
	}

	while(1){
		sigsuspend(&mask);
	}


	printf("Koniec Catchera\n");
	return 0;
}