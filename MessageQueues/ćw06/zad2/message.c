#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <mqueue.h>

#include "message.h"


void send_message(mqd_t mqdes, char* message,unsigned int prio)
{
	if (mq_send(mqdes, message, strlen(message), prio) == -1)
	{
		printf("Fail in mq_send()\n");
		puts(strerror(errno));
		exit(1);
	}
}


void receive_message(mqd_t mqdes, char* message,unsigned int* priop)
{
	if (mq_receive(mqdes, message, MAX_MSG_SIZE, priop) == -1){
		printf("Fail in mq_receive()\n");
		puts(strerror(errno));
		exit(1);
	}
}