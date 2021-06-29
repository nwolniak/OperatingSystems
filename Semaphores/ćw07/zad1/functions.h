#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define OVEN_AVAILABLE 0
#define OVEN_IDX 1
#define PIZZAS_OVEN 2
#define TABLE_AVAILABLE 3
#define TABLE_PIZZA_IN_IDX 4
#define PIZZAS_TABLE 5
#define TABLE_PIZZA_OUT_IDX 6


#define MAX_SIZE 5
typedef struct
{
	int pizza_n[MAX_SIZE];
}shm_block;


int get_sm();
int get_oven_shm();
int get_table_shm();
