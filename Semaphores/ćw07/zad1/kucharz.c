#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#include "functions.h"

int sm;
int oven_shm;
int table_shm;
int pizza_n;

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
	struct sembuf *sm_op = calloc(4, sizeof(struct sembuf));
	sm_op[0].sem_num = OVEN_AVAILABLE; // obsluga pieca
	sm_op[0].sem_op = 0;
	sm_op[0].sem_flg = 0;

	sm_op[1].sem_num = OVEN_AVAILABLE; // obsluga pieca
	sm_op[1].sem_op = 1;
	sm_op[1].sem_flg = 0;

	sm_op[2].sem_num = OVEN_IDX; // indeks wolnego miejsca w piecu
	sm_op[2].sem_op = 1;
	sm_op[2].sem_flg = 0;

	sm_op[3].sem_num = PIZZAS_OVEN; // liczba pizz w piecu
	sm_op[3].sem_op = 1;
	sm_op[3].sem_flg = 0;
	semop(sm, sm_op, 4);

	/* Kod chroniony */
	shm_block *oven = shmat(oven_shm, NULL, 0);
	int index = (semctl(sm, OVEN_IDX, GETVAL, NULL) - 1) % MAX_SIZE;
	oven->pizza_n[index] = pizza_n;
	int number_of_pizzas_in_oven = semctl(sm, PIZZAS_OVEN, GETVAL, NULL);
	printf("(%d %s) Dodałem pizze: %d. Liczba pizz w piecu: %d.\n", 
			getpid(), date(), pizza_n, number_of_pizzas_in_oven);
	shmdt(oven);

	/* Restore */
	struct sembuf *sm_op_restore = calloc(1, sizeof(struct sembuf));
	sm_op_restore[0].sem_num = OVEN_AVAILABLE; // obsluga pieca
	sm_op_restore[0].sem_op = -1;
	sm_op_restore[0].sem_flg = 0;
	semop(sm, sm_op_restore, 1);

	sleep((rand() % 2) + 4);
}

void take_out()
{
	/* Operations */
	struct sembuf *sm_op = calloc(3, sizeof(struct sembuf));
	sm_op[0].sem_num = OVEN_AVAILABLE; // obsluga pieca
	sm_op[0].sem_op = 0;
	sm_op[0].sem_flg = 0;

	sm_op[1].sem_num = OVEN_AVAILABLE; // obsluga pieca
	sm_op[1].sem_op = 1;
	sm_op[1].sem_flg = 0;

	sm_op[2].sem_num = PIZZAS_OVEN; // liczba pizz w piecu
	sm_op[2].sem_op = -1;
	sm_op[2].sem_flg = 0;
	semop(sm, sm_op, 3);

	/* Kod chroniony */
	int number_of_pizzas_in_oven = semctl(sm, PIZZAS_OVEN, GETVAL, NULL);
	int number_of_pizzas_on_table = semctl(sm, PIZZAS_TABLE, GETVAL, NULL);

	printf("(%d %s) Wyjmuję pizze: %d. Liczba pizz w piecu: %d. Liczba pizz na stole: %d.\n", 
			getpid(), date(), pizza_n, number_of_pizzas_in_oven, number_of_pizzas_on_table);

	/* Restore */
	struct sembuf *sm_op_restore = calloc(1, sizeof(struct sembuf));
	sm_op_restore[0].sem_num = OVEN_AVAILABLE; // obsluga pieca
	sm_op_restore[0].sem_op = -1;
	sm_op_restore[0].sem_flg = 0;
	semop(sm, sm_op_restore, 1);
}

void put_on_table()
{
	/* Operations */
	struct sembuf *sm_op = calloc(4, sizeof(struct sembuf));
	sm_op[0].sem_num = TABLE_AVAILABLE; // osbluga stolu
	sm_op[0].sem_op = 0;
	sm_op[0].sem_flg = 0;

	sm_op[1].sem_num = TABLE_AVAILABLE; // obsluga stolu
	sm_op[1].sem_op = 1;
	sm_op[1].sem_flg = 0;

	sm_op[2].sem_num = TABLE_PIZZA_IN_IDX; // indeks wolnego miejsca na stole
	sm_op[2].sem_op = 1;
	sm_op[2].sem_flg = 0;

	sm_op[3].sem_num = PIZZAS_TABLE; // liczba pizz na stole
	sm_op[3].sem_op = 1;
	sm_op[3].sem_flg = 0;
	semop(sm, sm_op, 4);

	/* Kod chroniony */
	shm_block *table = shmat(table_shm, NULL, 0);
	int index = (semctl(sm, TABLE_PIZZA_IN_IDX, GETVAL, NULL) - 1) % MAX_SIZE;
	table->pizza_n[index] = pizza_n;
	int number_of_pizzas_on_table = semctl(sm, PIZZAS_TABLE, GETVAL, NULL);
	printf("(%d %s) Położyłem pizze : %d. Liczba pizz na stole: %d.\n", 
			getpid(), date(), pizza_n, number_of_pizzas_on_table);
	shmdt(table);

	/* Restore */
	struct sembuf *sm_op_restore = calloc(1, sizeof(struct sembuf));
	sm_op_restore[0].sem_num = TABLE_AVAILABLE; // obsluga stolu
	sm_op_restore[0].sem_op = -1;
	sm_op_restore[0].sem_flg = 0;
	semop(sm, sm_op_restore, 1);
}


int main(int argc, char* argv[])
{
	srand(time(NULL));

	/* Semaphores */
	sm = get_sm();
	/* Oven shared memory */
	oven_shm = get_oven_shm();
	/* Table shared memory */
	table_shm = get_table_shm();

	while (1)
	{
		prepare_pizza();
		while (semctl(sm, PIZZAS_OVEN, GETVAL, NULL) == MAX_SIZE){}; // wait
		if (semctl(sm, PIZZAS_OVEN, GETVAL, NULL) < MAX_SIZE)
		{
		    add_pizza();
		}
		take_out();
		while (semctl(sm, PIZZAS_TABLE, GETVAL, NULL) == MAX_SIZE){}; // wait
		if (semctl(sm, PIZZAS_TABLE, GETVAL, NULL) < MAX_SIZE)
		{
			put_on_table();
		}
	}

	return 0;
}