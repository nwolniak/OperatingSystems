#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <signal.h>
#include <mqueue.h>
#include <errno.h>

#include "message.h"


#define MAX_CLIENTS 30
int server_queue;
int clients_number = 0;
int clients_queues[MAX_CLIENTS][3];


void self_remove_queue(){
	for (int i = 0 ; i < MAX_CLIENTS ; i++){
		if (clients_queues[i][0] != -1){
			char message[MAX_MSG_SIZE];
			sprintf(message, "%d", STOP);
			kill(clients_queues[i][2], SIGUSR1);
			send_message(clients_queues[i][0], message, STOP);
		}
	}
	mq_close(server_queue);
	mq_unlink("/server_queue");
	exit(0);
}


void sigint_handler(int signo){
	printf("\nServer handled sigint\n");
	self_remove_queue();
}


void init_handler(char* client_name){
	printf("Server got INIT command!\n");

	/* FIND AND SET UNIQUE CLIENT QUEUE ID */
	int client_number;
	int client_queue;

	char* c_name = calloc(30, sizeof(char));
	memcpy(c_name, client_name, strlen(client_name));
	printf("%s\n", client_name);
	if ((client_queue = mq_open(c_name, O_WRONLY)) == -1)
	{
		if (errno == ENOENT)
		{
			printf("to ten blad\n");
		}
		printf("Fail in mq_open()\n");
		puts(strerror(errno));
		self_remove_queue();
	}

	pid_t pid = atoi(strtok(client_name, "/client\n"));

	for (int i = 0 ; i < MAX_CLIENTS ; i++)
	{
		if (clients_queues[i][0] == -1)
		{
			clients_queues[i][0] = client_queue;
			clients_queues[i][2] = pid;
			client_number = i;
			clients_number++;
			break;
		}
		else if (i == MAX_CLIENTS - 1)
		{
			printf("Server reached max size of clients!\n");
			self_remove_queue();
		}
	}
	/* SEND UNIQUE CLIENT QUEUE ID TO A CLIENT */
	char message[MAX_MSG_SIZE];
	sprintf(message, "%d", client_number);
	send_message(client_queue, message, INIT);
}

void list_handler(int client_queue){
	printf("Server got LIST command\n");

	char message[MAX_MSG_SIZE];
	char list[MAX_MSG_SIZE];
	for (int i = 0 ; i < MAX_CLIENTS ; i++)
	{
		if (clients_queues[i][0] != -1){
			char tmp[6];
			sprintf(tmp, "%d %d\n", i, clients_queues[i][1] == -1 ? 1 : 0);
			strcat(list, tmp);
		}
	}
	strcpy(message, list);
	memset(list, 0 , MAX_MSG_SIZE);
	printf("SENDING LIST\n");
	send_message(client_queue, message, LIST);
}

void disconnect_handler(int client_id){
	printf("Server got DISCONNECT command\n");
	int connected_to = clients_queues[client_id][1];
	if (connected_to != -1)
	{
		char message[MAX_MSG_SIZE];
		sprintf(message, "%d", DISCONNECT);
		kill(clients_queues[connected_to][2], SIGUSR1);
		send_message(clients_queues[connected_to][0], message, DISCONNECT);
		clients_queues[connected_to][1] = -1;
		clients_queues[client_id][1] = -1;
	}
}

void stop_handler(int client_id){
	printf("Server got STOP command\n");

	mq_close(clients_queues[client_id][0]);

	int connected_to = clients_queues[client_id][1];
	if (connected_to != -1)
	{
		char message[MAX_MSG_SIZE];
		sprintf(message, "%d", DISCONNECT);
		send_message(clients_queues[connected_to][0], message, DISCONNECT);
		clients_queues[connected_to][1] = -1;
		clients_queues[client_id][1] = -1;
		clients_queues[client_id][0] = -1;
		clients_queues[client_id][2] = -1;
	}
	else
	{
		clients_queues[client_id][0] = -1;
		clients_queues[client_id][2] = -1;
	}
	clients_number--;
	if (clients_number == 0)
	{
		self_remove_queue();
	}
}

void connect_handler(int client_id, int client2_id){
	printf("Server got CONNECT command\n");

	char message1[MAX_MSG_SIZE];
	char message2[MAX_MSG_SIZE];

	int client_queue = clients_queues[client_id][0];
	int client2_queue = clients_queues[client2_id][0];

	if (client_id == client2_id || 
		(clients_queues[client_id][1] != -1 && clients_queues[client_id][1] != client2_id)|| 
		(clients_queues[client2_id][1] != -1 && clients_queues[client2_id][1] != client_id))
	{
		printf("Can not connect clients\n");
		sprintf(message1, "%d", -1);
		send_message(client_queue, message1, CONNECT);
	}
	else
	{
		clients_queues[client_id][1] = client2_id;
		clients_queues[client2_id][1] = client_id;

		char client_name[30];
		char client2_name[30];

		sprintf(client_name, "/client%d", clients_queues[client_id][2]);
		sprintf(client2_name, "/client%d", clients_queues[client2_id][2]); 


		sprintf(message1, "%s %d", client2_name, clients_queues[client2_id][2]);
		send_message(client_queue, message1, CONNECT);
		
		sprintf(message2, "%s %d", client_name, clients_queues[client_id][2]);
		kill(clients_queues[client2_id][2], SIGUSR1);
		send_message(client2_queue, message2, CONNECT);
		printf("Sended connecting signals\n");
	}
}

	


int main(int argc, char* argv[]){
	/* SERVER QUEUE */
	struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 5;
    attr.mq_msgsize = MAX_MSG_SIZE;
    attr.mq_curmsgs = 0;

	if ((server_queue = mq_open("/server_queue", O_RDONLY | O_CREAT , 0600, &attr)) == -1)
	{
		printf("Fail in mq_open()\n");
		puts(strerror(errno));
		return 1;
	}

	/* CLIENTS QUEUES */
	for (int i = 0 ; i < MAX_CLIENTS ; i++)
	{
		clients_queues[i][0] = -1; // QUEUE
		clients_queues[i][1] = -1; // AVAILABLE
		clients_queues[i][2] = -1; // PID
	}

	/* SIGINT SIGNAL HANDLING */
	if (signal(SIGINT, sigint_handler) == SIG_ERR)
	{
		printf("Error in signal()\n");
		return 1;
	}


	/* HANDLIND MESSAGE FROM SERVER QUEUE */
	char message[MAX_MSG_SIZE];
	unsigned int prio;
	while (1){
		receive_message(server_queue, message, &prio);
		char *type = strtok(message, " \n");
		if (prio == INIT)
		{
			init_handler(strtok(NULL, " \n"));	
		}
		else if (prio == LIST)
		{
			list_handler(atoi(strtok(NULL, " \n")));	
		}
		else if (prio == DISCONNECT)
		{
			disconnect_handler(atoi(strtok(NULL, " \n")));	
		}
		else if (prio == STOP)
		{
			stop_handler(atoi(strtok(NULL, " \n")));	
		}
		else if (prio == CONNECT)
		{
			char* client_id = strtok(NULL, " \n");
			char* client2_id = strtok(NULL, " \n");
			if (client_id == NULL || client2_id == NULL){
				printf("Fail in CONNECT command\n");
				self_remove_queue();
			}
			connect_handler(atoi(client_id), atoi(client2_id));
		}

	}

	return 0;
}