#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>

#include "message.h"


#define MAX_CLIENTS 30
int server_queue;
int clients_number = 0;
int clients_queues[MAX_CLIENTS][2];


void self_remove_queue(){
	for (int i = 0 ; i < MAX_CLIENTS ; i++){
		if (clients_queues[i][0] != -1){
			mymsg message;
			message.type = STOP;
			send_message(clients_queues[i][0], &message);
		}
	}
	msgctl(server_queue, IPC_RMID, NULL);
	exit(0);
}


void sigint_handler(int signo){
	printf("\nServer handled sigint\n");
	self_remove_queue();
	exit(0);
}


void init_handler(int client_queue){
	printf("Server got INIT command!\n");

	/* FIND AND SET UNIQUE CLIENT QUEUE ID */
	int client_number = 0;
	for (int i = 0 ; i < MAX_CLIENTS ; i++)
	{
		if (clients_queues[i][0] == -1)
		{
			clients_queues[i][0] = client_queue;
			client_number = i;
			clients_number++;
			break;
		}
		else if (i == MAX_CLIENTS - 1)
		{
			printf("Server reached max size of clients!\n");
			self_remove_queue();
			exit(1);
		}
	}
	/* SEND UNIQUE CLIENT QUEUE ID TO A CLIENT */
	mymsg r_message;
	r_message.type = INIT;
	sprintf(r_message.text, "%d", client_number);
	send_message(client_queue, &r_message);
}

void list_handler(int client_queue){
	printf("Server got LIST command\n");

	mymsg message;
	message.type = LIST;
	char list[TEXT_SIZE];
	for (int i = 0 ; i < MAX_CLIENTS ; i++)
	{
		if (clients_queues[i][0] != -1){
			char tmp[6];
			sprintf(tmp, "%d %d\n", i, clients_queues[i][1] == -1 ? 1 : 0);
			strcat(list, tmp);
		}
	}
	strcpy(message.text, list);
	memset(list, 0, TEXT_SIZE);
	send_message(client_queue, &message);
}

void disconnect_handler(int client_id){
	printf("Server got DISCONNECT command\n");
	int connected_to = clients_queues[client_id][1];
	if (connected_to != -1)
	{
		mymsg message;
		message.type = DISCONNECT;
		send_message(clients_queues[connected_to][0], &message);
		clients_queues[connected_to][1] = -1;
		clients_queues[client_id][1] = -1;
	}
}

void stop_handler(int client_id){
	printf("Server got STOP command\n");

	int connected_to = clients_queues[client_id][1];
	if (connected_to != -1)
	{
		clients_queues[connected_to][1] = -1;
		clients_queues[client_id][1] = -1;
		clients_queues[client_id][0] = -1;
	}
	else
	{
		clients_queues[client_id][0] = -1;
	}
	clients_number--;
	if (clients_number == 0)
	{
		self_remove_queue();
	}
}

void connect_handler(int client_id, int client2_id){
	printf("Server got CONNECT command\n");

	mymsg message1;
	message1.type = CONNECT;

	mymsg message2;
	message2.type = CONNECT;

	int client_queue = clients_queues[client_id][0];
	int client2_queue = clients_queues[client2_id][0];

	if (client_id == client2_id || 
		(clients_queues[client_id][1] != -1 && clients_queues[client_id][1] != client2_id)|| 
		(clients_queues[client2_id][1] != -1 && clients_queues[client2_id][1] != client_id))
	{
		printf("Can not connect clients\n");
		
		sprintf(message1.text, "%d", -1);
		send_message(client_queue, &message1);
	}
	else
	{
		clients_queues[client_id][1] = client2_id;
		clients_queues[client2_id][1] = client_id;

		sprintf(message1.text, "%d", client2_queue);
		send_message(client_queue, &message1);
		
		sprintf(message2.text, "%d", client_queue);
		send_message(client2_queue, &message2);
		printf("Sended connecting signals\n");
	}
}

	


int main(int argc, char* argv[]){

	/* SERVER KEY */
	key_t server_key;
	if ((server_key = ftok(getenv("HOME"), 's')) == -1)
	{
		printf("Fail in ftok()\n");
		puts(strerror(errno));
		return 1;
	}

	/* SERVER QUEUE */
	if ((server_queue = msgget(server_key, IPC_EXCL| IPC_CREAT | 0600)) == -1){
		printf("Fail in msgget()\n");
		puts(strerror(errno));
		return 1;
	}

	/* CLIENTS QUEUES */
	for (int i = 0 ; i < MAX_CLIENTS ; i++)
	{
		clients_queues[i][0] = -1;
		clients_queues[i][1] = -1;
	}

	/* SIGINT SIGNAL HANDLING */
	if (signal(SIGINT, sigint_handler) == SIG_ERR)
	{
		printf("Error in signal()\n");
		return 1;
	}


	/* HANDLIND MESSAGE FROM SERVER QUEUE */
	mymsg message;
	while (1){
		receive_message(server_queue, &message, -10);


		if (message.type == INIT)
		{
			init_handler(atoi(message.text));	
		}
		else if (message.type == LIST)
		{
			list_handler(atoi(message.text));	
		}
		else if (message.type == DISCONNECT)
		{
			disconnect_handler(atoi(message.text));	
		}
		else if (message.type == STOP)
		{
			stop_handler(atoi(message.text));	
		}
		else if (message.type == CONNECT)
		{
			char* client_id = strtok(message.text, " \n");
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
