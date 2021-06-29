#include <unistd.h> 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>


int main(int argc, char* argv[]) {
	if (argc != 3){
		printf("Invalid number of arguments!\n");
		return 1;
	}
	int producers = atoi(argv[1]);
	char* N = argv[2];

	mkfifo("pipe", S_IRWXU | S_IRWXG | S_IRWXO);

	/* CREATING PRODUCERS */
	for (int i = 1 ; i < producers + 1 ; i++){
		pid_t producer;

		if ((producer = fork()) == -1){
			printf("Fail in fork()\n");
			return 1;
		}
		else if (producer == 0)
		{
			char row[5];
			sprintf(row, "%d", i);
			char file_path[256];
			sprintf(file_path, "p%d.txt", i);
			fflush(stdout);
			execl("./producer", "./producer", "pipe", row, file_path, N,NULL);
			exit(1);
		}
	}

	/* CREATING CONSUMER */
	pid_t consumer;

	if ((consumer = fork()) == -1){
		printf("Fail in fork()\n");
			return 1;
	}
	else if(consumer == 0) 
	{
		char file_write[256];
		sprintf(file_write, "c%d.output", 1);
		char N_consumer[10];
			sprintf(N_consumer, "%d", atoi(N) + 2);
		execl("./consumer", "./consumer", "pipe", file_write, N_consumer, NULL);
	}

	return 0;
}
