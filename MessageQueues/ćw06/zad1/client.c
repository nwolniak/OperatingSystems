#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>

#include "message.h"
int server_queue;
int client_queue;
int client2_queue = -1;
int self_id;

void self_remove_queue(){
	msgctl(client_queue, IPC_RMID, NULL);
}

void stop_handler(){
	mymsg message;
	message.type = STOP;
	sprintf(message.text, "%d", self_id);
	send_message(server_queue, &message);
	self_remove_queue();
	printf("\nEND\n");
	exit(0);
}

void sigint_handler(int signo){
	stop_handler();
}

void list_handler(){
	mymsg message;
	message.type = LIST;
	sprintf(message.text, "%d", client_queue);
	send_message(server_queue, &message);
	receive_message(client_queue, &message, LIST);

	char* server_clients_info = strtok(message.text, "\n");
	do{
		printf("%s\n", server_clients_info);

	} while ((server_clients_info = strtok(NULL, "\n")) != NULL);
}

void disconnect_handler(){
	mymsg message;
	message.type = DISCONNECT;
	sprintf(message.text, "%d", self_id);
	send_message(server_queue, &message);
}

void chat_mode(int client2_queue){
	char buffer[TEXT_SIZE];
	mymsg message;
	message.type = CHAT;
	mymsg disconnect_message;
	disconnect_message.type = DISCONNECT;
	mymsg stop_message;
	stop_message.type = STOP;


	fd_set readfds;
    FD_ZERO(&readfds);

    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;

	while (client2_queue != -1)
	{
		FD_SET(STDIN_FILENO, &readfds);
		if (select(1, &readfds, NULL, NULL, &timeout)){
			if (fgets(buffer, MAX_MSG_SIZE, stdin) != NULL && strlen(buffer) > 1)
			{	
				char* token = strtok(buffer, "\n");
				if (strcmp(token, "DISCONNECT") == 0)
				{
					printf("Disconnecting ...\n");
					disconnect_handler();
					client2_queue = -1;
					break;
				}
				sprintf(message.text, "%s", token);
				send_message(client2_queue, &message);
			}
		}

		if (receive_message_nowait(client_queue, &message, CHAT) != -1)
		{
			printf("%s:%s\n", "Second client",message.text);
		}

		if (receive_message_nowait(client_queue, &disconnect_message, DISCONNECT) != -1)
		{
			printf("Disconnected\n");
			client2_queue = -1;
			break;
		}

		if (receive_message_nowait(client_queue, &stop_message, STOP) != -1)
		{
			printf("Killed by server\n");
			self_remove_queue();
			exit(0);
		}

	}
}

void connect_handler(int client2_id){
	mymsg message;
	message.type = CONNECT;
	sprintf(message.text, "%d %d", self_id, client2_id);
	send_message(server_queue, &message);
	receive_message(client_queue, &message, CONNECT);
	client2_queue = atoi(message.text);
	if (client2_queue != -1)
	{
		chat_mode(client2_queue);
	}
	else
	{
		printf("Cannot connect\n");
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
	if ((server_queue = msgget(server_key, 0)) == -1)
	{
		printf("Fail in msgget()\n");
		puts(strerror(errno));
		return 1;
	}

	/* CLIENT KEY */
	key_t client_key;
	if ((client_key = ftok(getenv("HOME"), getpid())) == -1)
	{
		printf("Fail in ftok()\n");
		puts(strerror(errno));
		return 1;
	}

	/* CLIENT QUEUE */
	if ((client_queue = msgget(client_key, IPC_EXCL| IPC_CREAT | 0600)) == -1)
	{
		printf("Fail in msgget()\n");
		puts(strerror(errno));
		return 1;
	}

	/* HANDLING SIGINT SIGNAL */
	if (signal(SIGINT, sigint_handler) == SIG_ERR)
	{
		printf("Error in signal()\n");
		return 1;
	}

	printf("REGISTERING\n");
	/* REGISTER TO A SERVER */
	mymsg r_message;
	r_message.type = INIT;
	sprintf(r_message.text, "%d", client_queue);
	send_message(server_queue, &r_message);

	/* RECEIVE UNIQUE ID FROM SERVER */
	receive_message(client_queue, &r_message, INIT);
	self_id = atoi(r_message.text);


	fd_set readfds;
    FD_ZERO(&readfds);

    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;

	char buffer[MAX_MSG_SIZE];
	mymsg message;
	message.type = CONNECT;
	while (1)
	{
		FD_SET(STDIN_FILENO, &readfds);
		/* HANDLE COMMAND LINE */
		if (select(1, &readfds, NULL, NULL, &timeout)){
			if (fgets(buffer, MAX_MSG_SIZE, stdin) != NULL && strlen(buffer) > 1)
			{
				char* token = strtok(buffer, " \n");
				if (strcmp(token, "LIST") == 0)
				{
					list_handler();
				}
				else if (strcmp(token, "CONNECT") == 0)
				{
					char* connect_to = strtok(NULL, " \n");
					if (connect_to != NULL)
					{
						connect_handler(atoi(connect_to));
					}
					else
					{
						printf("Invalid CONNECT command\n");
					}
					
				}
				else if (strcmp(token, "DISCONNECT") == 0)
				{
					disconnect_handler();
				}
				else if (strcmp(token, "STOP") == 0)
				{
					stop_handler();
				} 
				else
				{
					printf("Invalid command!\n");
				}
			}
		}

		if (receive_message_nowait(client_queue, &message, CONNECT) != -1)
		{
			printf("Connecting with other user\n");
			chat_mode(atoi(message.text));
		}

		if (receive_message_nowait(client_queue, &message, STOP) != -1)
		{
			printf("Killed by server\n");
			self_remove_queue();
			exit(0);
		}
	}

	return 0;
}