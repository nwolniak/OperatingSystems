#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#include "functions.h"

int pizza_n;
int sm;
int table_shm;


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
	struct sembuf *sm_op = calloc(4, sizeof(struct sembuf));
	sm_op[0].sem_num = TABLE_AVAILABLE; // obsluga stolu
	sm_op[0].sem_op = 0;
	sm_op[0].sem_flg = 0;

	sm_op[1].sem_num = TABLE_AVAILABLE; // obsluga stolu
	sm_op[1].sem_op = 1;
	sm_op[1].sem_flg = 0;

	sm_op[2].sem_num = PIZZAS_TABLE; // liczba pizz na stole
	sm_op[2].sem_op = -1;
	sm_op[2].sem_flg = 0;

	sm_op[3].sem_num = TABLE_PIZZA_OUT_IDX; // indeks pizzy do pobrania
	sm_op[3].sem_op = 1;
	sm_op[3].sem_flg = 0;
	semop(sm, sm_op, 4);

	/* Kod chroniony */
	shm_block *table = shmat(table_shm, NULL, 0);
	int index = (semctl(sm, TABLE_PIZZA_OUT_IDX, GETVAL, NULL) - 1) % MAX_SIZE;
	pizza_n = table->pizza_n[index];
	int number_of_pizzas_on_table = semctl(sm, PIZZAS_TABLE, GETVAL, NULL);
	printf("(%d %s) Pobieram pizze: %d Liczba pizz na stole: %d.\n", 
			getpid(), date(), pizza_n, number_of_pizzas_on_table);
	shmdt(table);

	/* Restore */
	struct sembuf *sm_op_restore = calloc(1, sizeof(struct sembuf));
	sm_op_restore[0].sem_num = TABLE_AVAILABLE; // obsluga stolu
	sm_op_restore[0].sem_op = -1;
	sm_op_restore[0].sem_flg = 0;
	semop(sm, sm_op_restore, 1);

	/* Pizza delivery */
	sleep((rand() % 2) + 4);
	printf("(%d %s) Dostarczam pizze: %d.\n", getpid(), date(), pizza_n);
	sleep((rand() % 2) + 4);
}


int main(int argc, char* argv[])
{
	srand(time(NULL));

	/* Semaphores */
	sm = get_sm();
	/* Table shared memory */
	table_shm = get_table_shm();

	while (1)
	{
		if (semctl(sm, PIZZAS_TABLE, GETVAL, NULL) > 0)
		{
		    take_pizza();
		}
	}

	return 0;
}