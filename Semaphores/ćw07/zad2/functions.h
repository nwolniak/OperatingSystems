#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <sys/time.h>
#include <unistd.h>

#define OVEN_AVAILABLE "/OVEN_AVAILABLE"
#define OVEN_IDX "/OVEN_IDX"
#define PIZZAS_OVEN "/PIZZAS_OVEN"
#define TABLE_AVAILABLE "/TABLE_AVAILABLE"
#define TABLE_PIZZA_IN_IDX "/TABLE_PIZZA_IN_IDX"
#define PIZZAS_TABLE "/PIZZAS_TABLE"
#define TABLE_PIZZA_OUT_IDX "/TABLE_PIZZA_OUT_IDX"

#define OVEN_SHM "/OVENSHM"
#define TABLE_SHM "/TABLE_SHM"


#define MAX_SIZE 5
typedef struct
{
	int pizza_n[MAX_SIZE];
}shm_block;