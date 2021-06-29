#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdint.h>
#include <signal.h>


pthread_t santa;
pthread_t reindeers[9];

pthread_mutex_t santa_queue = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t santa_start = PTHREAD_COND_INITIALIZER;
pthread_cond_t reindeers_start = PTHREAD_COND_INITIALIZER;

int reindeers_waiting = 0;


void sig_handler(int signo)
{
    pthread_exit(NULL);
}


void* santa_behavior()
{
    int gifts_delivered = 0;
    while (gifts_delivered < 3)
    {
        /* LOCK */
        pthread_mutex_lock(&santa_queue);

        /* WAIT FOR CONDITION */
        while (reindeers_waiting != 9)
        {
            pthread_cond_wait(&santa_start, &santa_queue);
        }

        printf("Mikołaj: budzę się\n");
        printf("Mikołaj: dostarczam zabawki\n");
        fflush(stdout);
        gifts_delivered++;
        reindeers_waiting = 0;
        pthread_cond_broadcast(&reindeers_start);

         /* UNLOCK */
        pthread_mutex_unlock(&santa_queue);

        sleep((rand() % 2) + 2);

        printf("Mikołaj: dostarczyłem zabawki\n");
        fflush(stdout);
    }

    // SEND END SIGNALS TO REINDEERS
    for (int i = 0 ; i < 9 ; i++)
    {
    pthread_kill(reindeers[i], SIGINT);
    }

    return NULL;
}

void* reindeer_behavior(void *index)
{
    int reindeer_idx = (intptr_t) index;

    while (1)
    {
        sleep((rand() % 5) + 5); // reindeer's vacations time

        /* LOCK */
        pthread_mutex_lock(&santa_queue);
        reindeers_waiting++;
        printf("Renifer: czeka %d reniferów na Mikołaja, %d\n", reindeers_waiting, reindeer_idx);
        fflush(stdout);

        if (reindeers_waiting == 9)
        {
            // SANTA ONLY ONE
            printf("Renifer: wybudzam Mikołaja, %d\n", reindeer_idx);
            pthread_cond_signal(&santa_start);
        }
        pthread_mutex_unlock(&santa_queue);
        

        /* LOCK */
        pthread_mutex_lock(&santa_queue);
        
        /* WAIT FOR CONDITION */
        while (reindeers_waiting != 0)
        {
            pthread_cond_wait(&reindeers_start, &santa_queue);
        }
        /* UNLOCK */
        pthread_mutex_unlock(&santa_queue);
        
        printf("Renifer: dostarczam zabawki, %d\n", reindeer_idx);
        fflush(stdout);
        sleep((rand() % 2) + 2);
        printf("Renifer: lecę na wakacje, %d\n", reindeer_idx);
        fflush(stdout);
    }
    return NULL;
}

int main(int argc, char* argv[])
{
    // SIGNAL HANDLER WHEN SANTA DELIVERED 3 GIFTS
    signal(SIGINT, sig_handler);

    if (pthread_create(&santa, NULL, santa_behavior, NULL) == -1)
    {
        perror("pthread_create santa");
        return 1;
    }

    for (int i = 0 ; i < 9 ; i++)
    {
        if (pthread_create(&reindeers[i], NULL, reindeer_behavior, (void*) (intptr_t) i) != 0)
        {
            perror("pthread_create reindeer");
            return 2;
        }
    }

    // WAIT FOR SANTA
    pthread_join(santa, NULL);

    // WAIT FOR REINDEERS
    for (int i = 0 ; i < 9 ; i++)
    {
        pthread_join(reindeers[i], NULL);
    }

    return 0;
}

