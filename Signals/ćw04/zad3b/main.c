#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>


pid_t catcher;
pid_t sender;

void handler(int signo){
	printf("Main handled %s signal\n", strsignal(signo));
	kill(SIGKILL, sender);
	kill(SIGKILL, catcher);
	exit(1);
}

int main(int argc, char* argv[]){
	if (argc != 3){
		printf("Invalid number of arguments!\n");
		return 1;
	}

	signal(SIGINT, handler);

	/* CATCHER */
	//pid_t catcher;
	if ((catcher = vfork()) == -1){
		printf("Error in fork()\n");
		puts(strerror(errno));
	}else if (catcher == 0){
		if (execl("/home/norbert/Desktop/NorbertWolniak/ćw04/zad3b/catcher",
					  "catcher",argv[2], NULL) == -1){
			printf("Fail in execl\n");
			puts(strerror(errno));
			exit(1);
		}
	}
	
	/* SENDER */
	//pid_t sender;
	if ((sender = vfork()) == -1){
		printf("Error in fork()\n");
		puts(strerror(errno));
	}else if (sender == 0){
		char catcher_str[12];
		sprintf(catcher_str, "%d", (int)catcher);
		if (execl("/home/norbert/Desktop/NorbertWolniak/ćw04/zad3b/sender",
					  "sender", catcher_str, argv[1], argv[2], NULL) == -1){
			printf("Fail in execl\n");
			puts(strerror(errno));
			exit(1);
		}
	} else{
		wait(0);
	}


	return 0;
}