#include "functions.h"

int get_sm()
{
    key_t sm_key = ftok(getenv("HOME"), 's');
    int sm_id;
    if ((sm_id = semget(sm_key, 0, 0)) == -1)
    {
        perror("get_sm");
        exit(1);
    }
    return sm_id;
}

int get_oven_shm()
{
    key_t shm_key = ftok(getenv("HOME"), 'o');
    int shm;
    if ((shm = shmget(shm_key, 0, 0)) == -1)
    {
        perror("get_oven_shm");
        exit(1);
    }
    return shm;
}

int get_table_shm()
{
    key_t shm_key = ftok(getenv("HOME"), 't');
    int shm;
    if ((shm = shmget(shm_key, 0, 0)) == -1)
    {
        perror("get_table_shm");
        exit(1);
    }
    return shm;
}