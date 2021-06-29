#include <mqueue.h>

#define STOP 6
#define DISCONNECT 5
#define LIST 4
#define INIT 3
#define CONNECT 2
#define CHAT 1

#define MAX_MSG_SIZE 500

void send_message(mqd_t mqdes, char* message, unsigned int prio);
void receive_message(mqd_t mqdes, char* message, unsigned int* priop);