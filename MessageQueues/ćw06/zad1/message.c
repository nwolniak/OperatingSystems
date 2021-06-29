#include <stdlib.h>
#include <stdio.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <string.h>
#include <errno.h>

#include "message.h"


void send_message(int msq_id, mymsg* message)
{
	if (msgsnd(msq_id, message, MAX_MSG_SIZE, 0) == -1)
	{
		printf("Fail in msgsnd()\n");
		puts(strerror(errno));
		exit(1);
	}
}


void receive_message(int msq_id, mymsg* message, int type)
{
	if (msgrcv(msq_id, message, MAX_MSG_SIZE, type, 0) == -1){
		printf("Fail in msgrcv()\n");
		puts(strerror(errno));
		exit(1);
	}
}

int receive_message_nowait(int msq_id, mymsg* message, int type)
{
	return msgrcv(msq_id, message, MAX_MSG_SIZE, type, IPC_NOWAIT);
}