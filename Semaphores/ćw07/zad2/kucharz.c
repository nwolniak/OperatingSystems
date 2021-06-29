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

sem_t* oven_available;
sem_t* oven_idx;
sem_t* pizzas_oven;
sem_t* table_available;
sem_t* table_pizza_in_idx;
sem_t* pizzas_table;
int oven_shm_fd;
int table_shm_fd;
int pizza_n;

void sig_handler(int signo)
{
	sem_close(oven_available);
	sem_close(oven_idx);
	sem_close(pizzas_oven);
	sem_close(table_available);
	sem_close(table_pizza_in_idx);
	sem_close(pizzas_table);

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


void prepare_pizza()
{
	pizza_n = rand() % 10;
	printf("(%d %s) Przygotowuje pizze: %d.\n", getpid(), date(), pizza_n);
	sleep((rand() % 2) + 1);
}

void add_pizza()
{	
	/* Operations */
	sem_wait(oven_available);
	sem_post(oven_idx);
	sem_post(pizzas_oven);


	/* Kod chroniony */
	shm_block *oven;
	if ((oven = mmap(NULL, sizeof(shm_block), PROT_WRITE, MAP_SHARED, oven_shm_fd, 0)) == (void *) -1)
	{
		perror("mmap");
		exit(1);
	}
	int index;
	sem_getvalue(oven_idx, &index);
	index = (index -1) % MAX_SIZE;
	oven->pizza_n[index] = pizza_n;
	int number_of_pizzas_in_oven;
	sem_getvalue(pizzas_oven, &number_of_pizzas_in_oven);
	printf("(%d %s) Dodałem pizze: %d. Liczba pizz w piecu: %d.\n", 
			getpid(), date(), pizza_n, number_of_pizzas_in_oven);
	munmap(oven, sizeof(shm_block));

	/* Restore */
	sem_post(oven_available);

	sleep((rand() % 2) + 4);
}

void take_out()
{
	/* Operations */
	sem_wait(oven_available);
	sem_wait(pizzas_oven);

	/* Kod chroniony */
	int number_of_pizzas_in_oven;
	sem_getvalue(pizzas_oven, &number_of_pizzas_in_oven);
	int number_of_pizzas_on_table;
	sem_getvalue(pizzas_table, &number_of_pizzas_on_table);

	printf("(%d %s) Wyjmuję pizze: %d. Liczba pizz w piecu: %d. Liczba pizz na stole: %d.\n", 
			getpid(), date(), pizza_n, number_of_pizzas_in_oven, number_of_pizzas_on_table);

	/* Restore */
	sem_post(oven_available);
}

void put_on_table()
{
	/* Operations */
	sem_wait(table_available);
	sem_post(table_pizza_in_idx);
	sem_post(pizzas_table);

	/* Kod chroniony */
	shm_block *table;
	if ((table = mmap(NULL, sizeof(shm_block), PROT_WRITE, MAP_SHARED, table_shm_fd, 0)) == (void *) -1)
	{
		perror("mmap");
		exit(1);
	}
	int index;
	sem_getvalue(table_pizza_in_idx, &index);
	index = (index -1) % MAX_SIZE;
	table->pizza_n[index] = pizza_n;
	int number_of_pizzas_on_table;
	sem_getvalue(pizzas_table, &number_of_pizzas_on_table);
	printf("(%d %s) Położyłem pizze : %d. Liczba pizz na stole: %d.\n", 
			getpid(), date(), pizza_n, number_of_pizzas_on_table);
	munmap(table, sizeof(shm_block));

	/* Restore */
	sem_post(table_available);
}


int main(int argc, char* argv[])
{
	signal(SIGINT, sig_handler);
	srand(time(NULL));

	/* Semaphores */
	oven_available = sem_open(OVEN_AVAILABLE, O_RDWR);
	oven_idx = sem_open(OVEN_IDX, O_RDWR);
	pizzas_oven = sem_open(PIZZAS_OVEN, O_RDWR);
	table_available = sem_open(TABLE_AVAILABLE, O_RDWR);
	table_pizza_in_idx = sem_open(TABLE_PIZZA_IN_IDX, O_RDWR);
	pizzas_table = sem_open(PIZZAS_TABLE, O_RDWR);

	/* Oven shared memory fd*/
	oven_shm_fd = shm_open(OVEN_SHM, O_RDWR, 0777);

	/* Table shared memory fd*/
	table_shm_fd = shm_open(TABLE_SHM, O_RDWR, 0777);


	int pizzas_in_oven;
	int pizzas_on_table;
	while (1)
	{
		prepare_pizza();
		do{
			sem_getvalue(pizzas_oven, &pizzas_in_oven);
			if (pizzas_in_oven < MAX_SIZE)
			{
				add_pizza();
			}
		}while (pizzas_in_oven == MAX_SIZE); // wait
		take_out();
		do{
			sem_getvalue(pizzas_table, &pizzas_on_table);
			if (pizzas_on_table < MAX_SIZE)
			{
				put_on_table();
			}
		}while (pizzas_on_table == MAX_SIZE); // wait
	}
	return 0;
}