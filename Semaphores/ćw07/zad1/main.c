#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/time.h>
#include <unistd.h>

#include "functions.h"


int sm;
int oven_shm;
int table_shm;
pid_t* children;
int n_children = 0;

void self_remove()
{
	semctl(sm, 0, IPC_RMID, NULL);
    shmctl(oven_shm, IPC_RMID, NULL);
    shmctl(table_shm, IPC_RMID, NULL);
    exit(0);
}

void sig_handler()
{	
	printf("\nEND\n");
	for (int i = 0 ; i < n_children ; i++)
	{
		kill(children[i], SIGKILL);
	}
	self_remove();
}

int main(int argc, char* argv[])
{
	signal(SIGINT, sig_handler);

	if (argc != 3)
	{
		printf("Invalid number of arguments!");
		return 1;
	}

	/* Semaphores */
	key_t sm_key = ftok(getenv("HOME"), 's');
    if ((sm = semget(sm_key, 7, IPC_CREAT | 0777)) == -1)
    {
    	perror("Semget");
    	return 2;
    }

    union semun {
        int val;
        struct semid_ds *buf;
        unsigned short *array;
        struct seminfo *__buf;
    };

    union semun arg;
    arg.val = 0;
    for (int i = 0; i < 7; i++)
    {
        semctl(sm, i, SETVAL, arg);
    }

    /* Oven shared memory */
    key_t oven_shm_key = ftok(getenv("HOME"), 'o');
    if ((oven_shm = shmget(oven_shm_key, sizeof(shm_block), IPC_CREAT | 0666)) == -1)
    {
    	perror("Shmget oven");
    	return 3;
    }

    /* Table shared memory */
    key_t table_shm_key = ftok(getenv("HOME"), 't');
    if ((table_shm = shmget(table_shm_key, sizeof(shm_block), IPC_CREAT | 0666)) == -1)
    {
    	perror("Shmget table");
    	return 3;
    }

    children = calloc(atoi(argv[1]) + atoi(argv[2]), sizeof(pid_t));

    for (int i = 0 ; i < atoi(argv[1]) ; i++)
    {
        pid_t kucharz;
        if ((kucharz = fork()) == -1)
        {
        	perror("Fork kucharz");
        	return 4;
        }
        else if (kucharz == 0)
        {
        	children[i] = kucharz;
        	n_children++;
        	execlp("./kucharz", "./kucharz", NULL);
        }
    }

    for (int i = 0 ; i < atoi(argv[2]) ; i++)
    {
        pid_t dostawca;
        if ((dostawca = fork()) == -1)
        {
        	perror("Fork dostawca");
        	return 5;
        }
        else if (dostawca == 0)
        {
        	children[atoi(argv[1]) + i] = dostawca;
        	n_children++;
        	execlp("./dostawca", "./dostawca", NULL);
        }
    }
    wait(0); // procesy potomne zabijane CTR+C

    self_remove();
	return 0;
}
