#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>

#include "functions.h"

sem_t* table_available;
sem_t* table_pizza_in_idx;
sem_t* pizzas_table;
sem_t* table_pizza_out_idx;
int table_shm_fd;
int pizza_n;

void sig_handler(int signo)
{
	sem_close(table_available);
	sem_close(table_pizza_in_idx);
	sem_close(pizzas_table);
	sem_close(table_pizza_out_idx);

	exit(0);
}


char* date()
{
  	struct timeval time_now;
	gettimeofday(&time_now, NULL);
	struct tm* tim_tm = gmtime(&time_now.tv_sec);

	char* buffer = calloc(30, sizeof(char));
	sprintf(buffer, "%02i:%02i:%02i:%02li",
			tim_tm->tm_hour,
			tim_tm->tm_min,
			tim_tm->tm_sec,
			time_now.tv_usec);
	return buffer;
}

void take_pizza()
{
	/* Operations */
	sem_wait(table_available);
	sem_wait(pizzas_table);
	sem_post(table_pizza_out_idx);


	/* Kod chroniony */
	shm_block *table;
	if ((table = mmap(NULL, sizeof(shm_block), PROT_WRITE, MAP_SHARED, table_shm_fd, 0)) == (void *) -1)
	{
		perror("mmap");
		exit(1);
	}
	int index;
	sem_getvalue(table_pizza_out_idx, &index);
	index = (index -1) % MAX_SIZE;
	pizza_n = table->pizza_n[index];
	int number_of_pizzas_on_table;
	sem_getvalue(pizzas_table, &number_of_pizzas_on_table);
	printf("(%d %s) Pobieram pizze: %d Liczba pizz na stole: %d.\n", 
			getpid(), date(), pizza_n, number_of_pizzas_on_table);
	munmap(table, sizeof(shm_block));


	/* Restore */
	sem_post(table_available);

	/* Pizza delivery */
	sleep((rand() % 2) + 4);
	printf("(%d %s) Dostarczam pizze: %d.\n", getpid(), date(), pizza_n);
	sleep((rand() % 2) + 4);
}


int main(int argc, char* argv[])
{
	signal(SIGINT, sig_handler);
	srand(time(NULL));

	/* Semaphores */
	table_available = sem_open(TABLE_AVAILABLE, O_RDWR);
	table_pizza_in_idx = sem_open(TABLE_PIZZA_IN_IDX, O_RDWR);
	pizzas_table = sem_open(PIZZAS_TABLE, O_RDWR);
	table_pizza_out_idx = sem_open(TABLE_PIZZA_OUT_IDX, O_RDWR);

	/* Table shared memory */
	table_shm_fd = shm_open(TABLE_SHM, O_RDWR, 0777);

	int pizzas_on_table;
	while (1)
	{
		sem_getvalue(pizzas_table, &pizzas_on_table);
		if (pizzas_on_table > 0)
		{
			take_pizza();
		}
	}

	return 0;
}