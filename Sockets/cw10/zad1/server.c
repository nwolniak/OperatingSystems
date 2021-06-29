#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/un.h>
#include <pthread.h>
#include <signal.h>
#include <poll.h>
#include <unistd.h>
#include <errno.h>

#define MAX_CLIENTS 20
#define MAX_MSG_LENGTH 256

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

char** clients_names;
int clients_fds[MAX_CLIENTS][3]; // client | offline/online | opponent
int clients_number = 0;

void sig_handler(int signo)
{
    for (int i = 0 ; i < MAX_CLIENTS ; i++)
    {
        if (clients_fds[i][0] != -1 && clients_fds[i][1] == 1)
        {
            send(clients_fds[i][0], "quit", MAX_MSG_LENGTH, 0); 
        }
    }
    exit(0);
}

int add_client(char *name, int fd)
{
    int client_index = 0;
    for (int i = 0; i < MAX_CLIENTS; i++)
    {

        if (clients_fds[i][0] != -1 && strcmp(clients_names[i], name) == 0)
        {
            printf("%s\n", clients_names[i]);
            printf("Client exists!\n");
            return -1;
        }
        else if (clients_fds[i][0] == -1)
        {
            client_index = i;
            break;
        }
    }

    if (client_index == MAX_CLIENTS - 1)
    {
        printf("Max clients!");
        return -1;
    }

    char* client_name = calloc(MAX_MSG_LENGTH, sizeof(char));
    strcpy(client_name, name);
    clients_names[client_index] = client_name;
    clients_fds[client_index][0] = fd;
    clients_fds[client_index][1] = 1; // offline | online
    printf("%s\n", clients_names[client_index]);
    clients_number++;
    return client_index;
}

void remove_client(int client_id)
{
    clients_names[client_id] = NULL;
    clients_fds[client_id][0] = 0;
    clients_fds[client_id][1] = 0; // offline
    clients_number--;
    /* CHECK FOR OPPONENT */
    if (clients_fds[client_id][2] != -1)
    {
        send(clients_fds[client_id][2], "quit", MAX_MSG_LENGTH, 0);
        clients_fds[client_id][2] = -1;
    }
    clients_number--;
}


void* ping()
{
    while (1)
    {
        pthread_mutex_lock(&mutex);
        for (int i = 0; i < MAX_CLIENTS; i++)
        {
            /* IF OFFLINE */
            if (clients_fds[i][0] != -1 && clients_fds[i][1] == 0)
            {
                remove_client(i);
            }
        }
        for (int i = 0; i < MAX_CLIENTS; i++)
        {
            /* IF ONLINE */
            if (clients_fds[i][0] != -1 && clients_fds[i][1] == 1)
            { 
                send(clients_fds[i][0], "ping", MAX_MSG_LENGTH, 0);
            }
        }
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
    return NULL;
}

int init_local_socket(char *path)
{
    int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, path);
    unlink(path);
    if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) == -1)
    {
        perror("bind local");
        exit(1);
    }
    listen(sockfd, MAX_CLIENTS);
    return sockfd;
}

int init_network_socket(char *port)
{
    struct addrinfo *info;
    struct addrinfo hints;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    getaddrinfo(NULL, port, &hints, &info);
    int sockfd = socket(info->ai_family, info->ai_socktype, info->ai_protocol);
    if (bind(sockfd, info->ai_addr, info->ai_addrlen) == -1)
    {
        perror("bind network");
        exit(1);
    }
    listen(sockfd, MAX_CLIENTS);
    freeaddrinfo(info);

    return sockfd;
}

int get_message(int local_socket, int network_socket)
{
    struct pollfd *fds = calloc(clients_number + 2, sizeof(struct pollfd));
    fds[0].fd = local_socket;
    fds[0].events = POLLIN;
    fds[1].fd = network_socket;
    fds[1].events = POLLIN;
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < clients_number; i++)
    {
        fds[i + 2].fd = clients_fds[i][0];
        fds[i + 2].events = POLLIN;
    }
    pthread_mutex_unlock(&mutex);
    poll(fds, clients_number + 2, -1);
    int fd;
    for (int i = 0; i < clients_number + 2; i++)
    {
        if (fds[i].revents & POLLIN)
        {
            fd = fds[i].fd;
            break;
        }
    }
    if (fd == local_socket || fd == network_socket)
    {
        fd = accept(fd, NULL, NULL);
    }
    free(fds);

    return fd;
}

int main(int argc, char *argv[])
{
    /* SIGNAL HANDLER */
    signal(SIGINT, sig_handler);

    /* ARGS */
    if (argc != 3)
    {   
        printf("Invalid number of arguments!\n");
        return 1;
    }
    char *port = argv[1];
    char *socket_path = argv[2];

    int local_socket = init_local_socket(socket_path);
    int network_socket = init_network_socket(port);
    
    
    /* INIT GLOBAL VARIABLES */
    clients_names = calloc(MAX_CLIENTS, sizeof(char*));
    for (int i = 0 ; i < MAX_CLIENTS ; i++)
    {
        clients_names[i] = calloc(256, sizeof(char));
        clients_fds[i][0] = -1;
        clients_fds[i][1] = -1;
        clients_fds[i][2] = -1;
    }


    pthread_t pinging_thread;
    pthread_create(&pinging_thread, NULL, ping, NULL);

    while (1)
    {
        int client_fd = get_message(local_socket, network_socket);
        char buffer[MAX_MSG_LENGTH];
        recv(client_fd, buffer, MAX_MSG_LENGTH, 0);
        char *command = strtok(buffer, " ");
        char *client_name = strtok(NULL, " ");
        pthread_mutex_lock(&mutex);
        if (strcmp(command, "register") == 0)
        {
            int index = add_client(client_name, client_fd);

            if (index == -1)
            {
                send(client_fd, "wait invalid_name", MAX_MSG_LENGTH, 0);
                close(client_fd);
            }
            else
            {
                sprintf(buffer, "wait %d", index);
                send(client_fd, buffer, MAX_MSG_LENGTH, 0);
            }
            

            if (clients_number % 2 == 0)
            {
                
                int opponent;
                for (int i = 0 ; i <  MAX_CLIENTS ; i++)
                {
                    if (i != index && clients_fds[i][0] != -1 && clients_fds[i][2] == -1)
                    {
                        opponent = i;
                        clients_fds[index][2] = clients_fds[opponent][0];
                        clients_fds[opponent][2] = clients_fds[index][0];
                    }
                }
                send(clients_fds[index][0], "start O", MAX_MSG_LENGTH, 0);
                send(clients_fds[opponent][0], "start X", MAX_MSG_LENGTH, 0);
            }
        }
        if (strcmp(command, "move") == 0)
        {
            char *move = strtok(NULL, " ");
            int move_idx = atoi(move);
            int client_id = atoi(client_name);
            sprintf(buffer, "move %d", move_idx);
            send(clients_fds[client_id][2], buffer, MAX_MSG_LENGTH, 0);
        }
        if (strcmp(command, "quit") == 0)
        {
            remove_client(atoi(client_name));
        }
        if (strcmp(command, "ping") == 0)
        {
            int client_id = atoi(client_name);
            if (client_id == -1)
            {
                clients_fds[client_id][1] = 0; // offline
            }
        }
        pthread_mutex_unlock(&mutex);
    }
    return 0;
}
