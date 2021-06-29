#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <unistd.h>
#include <signal.h>
#include "functions.h"

pid_t* children;
int n_children = 0;

void self_remove()
{   
    sem_unlink(OVEN_AVAILABLE);
    sem_unlink(OVEN_IDX);
    sem_unlink(PIZZAS_OVEN);
    sem_unlink(TABLE_AVAILABLE);
    sem_unlink(TABLE_PIZZA_IN_IDX);
    sem_unlink(PIZZAS_TABLE);
    sem_unlink(TABLE_PIZZA_OUT_IDX);

	if (shm_unlink(OVEN_SHM) == -1)
    {
        perror("Shm_unlink");
        exit(1);
    }
    if (shm_unlink(TABLE_SHM) == -1)
    {
        perror("Shm_unlink");
        exit(1);
    }
    exit(0);
}

void sig_handler(int signo)
{	
	printf("\nEND\n");
	for (int i = 0 ; i < n_children ; i++)
	{
		kill(children[i], SIGINT);
	}
    free(children);
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
    sem_t* sm;
    if ((sm = sem_open(OVEN_AVAILABLE, O_CREAT | O_RDWR, 0777, 1)) == SEM_FAILED)
    {
    	perror("Sem_open");
    	return 2;
    }
    sem_close(sm);

    if ((sm = sem_open(OVEN_IDX, O_CREAT | O_RDWR, 0777, 0)) == SEM_FAILED)
    {
        perror("Sem_open");
        return 3;
    }
    sem_close(sm);

    if ((sm = sem_open(PIZZAS_OVEN, O_CREAT | O_RDWR, 0777, 0)) == SEM_FAILED)
    {
        perror("Sem_open");
        return 4;
    }
    sem_close(sm);

    if ((sm = sem_open(TABLE_AVAILABLE, O_CREAT | O_RDWR, 0777, 1)) == SEM_FAILED)
    {
        perror("Sem_open");
        return 5;
    }
    sem_close(sm);

    if ((sm = sem_open(TABLE_PIZZA_IN_IDX, O_CREAT | O_RDWR, 0777, 0)) == SEM_FAILED)
    {
        perror("Sem_open");
        return 6;
    }
    sem_close(sm);

    if ((sm = sem_open(PIZZAS_TABLE, O_CREAT | O_RDWR, 0777, 0)) == SEM_FAILED)
    {
        perror("Sem_open");
        return 7;
    }
    sem_close(sm);

    if ((sm = sem_open(TABLE_PIZZA_OUT_IDX, O_CREAT | O_RDWR, 0777, 0)) == SEM_FAILED)
    {
        perror("Sem_open");
        return 8;
    }
    sem_close(sm);


    /* Oven shared memory */
    int oven_fd;
    if ((oven_fd = shm_open(OVEN_SHM, O_CREAT | O_RDWR, 0777)) == -1)
    {
        perror("Shm_open");
        return 9;
    }
    if (ftruncate(oven_fd, sizeof(shm_block)) == -1)
    {
        perror("ftruncate");
        return 10;
    }

    /* Table shared memory */
    int table_fd;
    if ((table_fd = shm_open(TABLE_SHM, O_CREAT | O_RDWR, 0777)) == -1)
    {
        perror("Shm_open");
        return 11;
    }
    if (ftruncate(table_fd, sizeof(shm_block)) == -1)
    {
        perror("ftruncate");
        return 12;
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
    wait(0); // procesy potomne zabijane CRL+C

    self_remove();
	return 0;
}