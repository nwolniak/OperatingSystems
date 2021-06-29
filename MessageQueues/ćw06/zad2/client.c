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

int server_queue;
int client_queue;
int client2_queue = -1;
int self_id;
char client_name[30];

void self_remove_queue(){
	mq_close(server_queue);
	mq_close(client_queue);

	if (client2_queue != -1)
	{
		mq_close(client2_queue);
	}

	mq_unlink(client_name);
}

void stop_handler(){
	char message[MAX_MSG_SIZE];
	sprintf(message, "%d %d", STOP, self_id);
	send_message(server_queue, message, STOP);
	self_remove_queue();
	printf("\nEND\n");
	exit(0);
}

void sigint_handler(int signo){
	stop_handler();
}

void list_handler(){
	printf("LIST HANDLER\n");
	char message_to[MAX_MSG_SIZE];
	sprintf(message_to, "%d %d", LIST, client_queue);
	send_message(server_queue, message_to, LIST);
	unsigned int prio;
	char message_back[MAX_MSG_SIZE];
	receive_message(client_queue, message_back, &prio);

	char* server_clients_info = strtok(message_back, "\n");
	do{
		printf("%s\n", server_clients_info);

	} while ((server_clients_info = strtok(NULL, "\n")) != NULL);
}

void disconnect_handler(){
	char message[MAX_MSG_SIZE];
	sprintf(message, "%d %d", DISCONNECT, self_id);
	send_message(server_queue, message, DISCONNECT);
}

void chat_mode(int client2_pid){
	printf("IN CHAT MODE\n");

	fd_set readfds;
    FD_ZERO(&readfds);

    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;

	while (client2_queue != -1)
	{
		FD_SET(STDIN_FILENO, &readfds);
		if (select(1, &readfds, NULL, NULL, &timeout)){
			char buffer[MAX_MSG_SIZE];
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

				if (client2_queue == -1) break;
				char message[MAX_MSG_SIZE];
				sprintf(message, "%s", token);
				kill(client2_pid, SIGUSR1);
				send_message(client2_queue, message, CHAT);
				memset(message, 0, MAX_MSG_SIZE);
				memset(buffer, 0, MAX_MSG_SIZE);
			}
		}
	}
	mq_close(client2_queue);
	printf("Exiting chat mode ...\n");
}

void connect_handler(int client2_id){
	char message_to[MAX_MSG_SIZE];
	sprintf(message_to, "%d %d %d", CONNECT, self_id, client2_id);
	send_message(server_queue, message_to, CONNECT);

	char message_back[MAX_MSG_SIZE];
	unsigned int prio;
	receive_message(client_queue, message_back, &prio);
	char* client2_name = strtok(message_back, " \n");
	client2_queue = mq_open(client2_name, O_WRONLY);
	int client2_pid = atoi(strtok(NULL, " \n"));
	if (client2_queue != -1)
	{
		chat_mode(client2_pid);
	}
	else
	{
		printf("Cannot connect\n");
	}	
}

void sigusr1_handler(int signo){
	printf("HANDLED SIGUSR1 SIGNAL\n");
	char message[MAX_MSG_SIZE];
	unsigned int prio;
	receive_message(client_queue, message, &prio);
	if (prio == CONNECT)
	{
		printf("Connecting with other user\n");
		char* client2_name = strtok(message, " \n");
		client2_queue = mq_open(client2_name, O_WRONLY);
		int client2_pid = atoi(strtok(NULL, " \n"));
		if (client2_queue != -1){
			chat_mode(client2_pid);
		}
		else
		{
			printf("Cannot connect\n");
		}
	}
	else if (prio == CHAT)
	{
		printf("%s:%s\n", "Second client", message);
	}
	else if (prio == DISCONNECT)
	{
		printf("Disconnected\n");
		client2_queue = -1;
	}
	else if (prio == STOP)
	{
		printf("Killed by server\n");
		stop_handler();
		exit(0);
	}
	
}

int main(int argc, char* argv[]){
	/* SERVER QUEUE */
	if ((server_queue = mq_open("/server_queue", O_WRONLY)) == -1)
	{
		printf("Fail in mq_open()\n");
		puts(strerror(errno));
		return 1;
	}

	/* CLIENT QUEUE */
	struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 5;
    attr.mq_msgsize = MAX_MSG_SIZE;
    attr.mq_curmsgs = 0;


    sprintf(client_name, "/client%d", getpid());
    printf("%s %d\n", "pid", getpid());

    if ((client_queue = mq_open(client_name, O_RDONLY  | O_CREAT | O_EXCL, 0777, &attr)) == -1)
    {
    	printf("Fail in mq_open()\n");
		puts(strerror(errno));
		return 1;
    }

	/* HANDLING SIGINT SIGNAL */
	if (signal(SIGINT, sigint_handler) == SIG_ERR)
	{
		printf("Error in signal() sigint\n");
		return 1;
	}

	/* HANDLING SIGUSR1 SIGNAL TO RECEIVE A MESSAGE*/
	struct sigaction act;
	act.sa_handler = sigusr1_handler;
	act.sa_flags = SA_RESTART;
	if (sigaction(SIGUSR1, &act, NULL) == -1){
		printf("Error in sigaction()\n");
		puts(strerror(errno));
		return 1;
	}


	printf("REGISTERING\n");
	/* REGISTER TO A SERVER */
	char init_message_to[MAX_MSG_SIZE];
	printf("%s\n", client_name);
    sprintf(init_message_to, "%d %s", INIT, client_name);
    send_message(server_queue, init_message_to, INIT);

	/* RECEIVE UNIQUE ID FROM SERVER */
	char init_message_back[MAX_MSG_SIZE];
    unsigned int prio;
    receive_message(client_queue, init_message_back, &prio);
    self_id = atoi(strtok(init_message_back, " \n"));

    printf("REGISTERED with id %d\n", self_id);
	char buffer[MAX_MSG_SIZE];
	while (1)
	{
		/* HANDLE COMMAND LINE */
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

	return 0;
}