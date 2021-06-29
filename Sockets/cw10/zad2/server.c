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

#define MAX_CLIENTS 10
#define MAX_MSG_LENGTH 256

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

char** clients_names;
int clients_fds[MAX_CLIENTS][4]; // client | offline/online | opponent | opponent idx
struct sockaddr clients_addrs[MAX_CLIENTS];
int clients_number = 0;

void sig_handler(int signo)
{
    for (int i = 0 ; i < MAX_CLIENTS ; i++)
    {
        if (clients_fds[i][0] != -1 && clients_fds[i][1] == 1)
        {
            sendto(clients_fds[i][0], "quit", MAX_MSG_LENGTH, 0, &clients_addrs[i], sizeof(struct sockaddr));
        }
    }
    exit(0);
}


int add_client(char *name, int fd, struct sockaddr addr)
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
        printf("Max clients!\n");
        return -1;
    }

    char* client_name = calloc(256, sizeof(char));
    strcpy(client_name, name);
    clients_names[client_index] = client_name;
    clients_fds[client_index][0] = fd;
    clients_fds[client_index][1] = 1; // offline | online
    clients_addrs[client_index] = addr;
    printf("%s\n", clients_names[client_index]);
    clients_number++;
    return client_index;
}

void remove_client(int client_id)
{
    clients_names[client_id] = NULL;
    clients_fds[client_id][0] = 0;
    clients_fds[client_id][1] = 0; // offline
    memset(&clients_addrs[client_id], 0, sizeof(struct sockaddr));
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
            /* IF OFFLINE REMOVE */
            if (clients_fds[i][0] != -1 && clients_fds[i][1] == 0)
            {
                remove_client(i);
            }
        }
        for (int i = 0; i < MAX_CLIENTS; i++)
        {
            /* IF ONLINE PING */
            if (clients_fds[i][0] != -1 && clients_fds[i][1] == 1)
            { 
                sendto(clients_fds[i][0], "ping", MAX_MSG_LENGTH, 0, &clients_addrs[i], sizeof(struct sockaddr));
            }
        }
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
    return NULL;
}

int init_local_socket(char *path)
{
    int sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, path);
    unlink(path);
    if (bind(sockfd, (struct sockaddr*)&addr, sizeof(struct sockaddr_un)) == -1)
    {
        perror("bind local");
        exit(1);
    }
    return sockfd;
}

int init_network_socket(char *port)
{
    struct addrinfo *info;
    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;
    getaddrinfo(NULL, port, &hints, &info);
    int sockfd = socket(info->ai_family, info->ai_socktype, info->ai_protocol);
    if (bind(sockfd, info->ai_addr, info->ai_addrlen) == -1)
    {
        perror("bind network");
        exit(1);
    }
    freeaddrinfo(info);

    return sockfd;
}

int get_message(int local_socket, int network_socket)
{
    struct pollfd* fds = calloc(2, sizeof(struct pollfd));
    fds[0].fd = local_socket;
    fds[0].events = POLLIN;
    fds[1].fd = network_socket;
    fds[1].events = POLLIN;
    poll(fds, 2, -1);
    for (int i = 0; i < 2; i++)
    {
        if (fds[i].revents & POLLIN)
        {
            return fds[i].fd;
        }
    }
    return -1;
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
    char *path = argv[2];

    int local_socket = init_local_socket(path);
    int network_socket = init_network_socket(port);
    
    
    /* INIT GLOBAL VARIABLES */
    clients_names = calloc(MAX_CLIENTS, sizeof(char*));
    for (int i = 0 ; i < MAX_CLIENTS ; i++)
    {
        clients_names[i] = calloc(256, sizeof(char));
        clients_fds[i][0] = -1;
        clients_fds[i][1] = -1;
        clients_fds[i][2] = -1;
        clients_fds[i][3] = -1;
        memset(&clients_addrs[i], 0, sizeof(struct sockaddr_in));
    }


    pthread_t pinging_thread;
    pthread_create(&pinging_thread, NULL, ping, NULL);
    while (1)
    {
        int client_fd = get_message(local_socket, network_socket);
        char buffer[MAX_MSG_LENGTH];
        struct sockaddr from_addr;
        socklen_t from_length = sizeof(struct sockaddr);
        recvfrom(client_fd, buffer, MAX_MSG_LENGTH, 0, &from_addr, &from_length);

        char *command = strtok(buffer, " ");
        char *client_name = strtok(NULL, " ");
        pthread_mutex_lock(&mutex);
        if (strcmp(command, "register") == 0)
        {
            int index = add_client(client_name, client_fd, from_addr);

            if (index == -1)
            {
                sendto(client_fd, "wait invalid_name", MAX_MSG_LENGTH, 0, (struct sockaddr*)&from_addr, sizeof(struct sockaddr));
            }
            else
            {
                sprintf(buffer, "wait %d", index);
                sendto(client_fd, buffer, MAX_MSG_LENGTH, 0, (struct sockaddr*)&from_addr, sizeof(struct sockaddr));
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
                        clients_fds[index][3] = opponent; // set opponent index
                        clients_fds[opponent][3] = index; // set opponent index
                    }
                }
                sendto(clients_fds[index][0], "start O", MAX_MSG_LENGTH, 0, &clients_addrs[index], sizeof(struct sockaddr));
                sendto(clients_fds[opponent][0], "start X", MAX_MSG_LENGTH, 0, &clients_addrs[opponent], sizeof(struct sockaddr));
            }
        }
        if (strcmp(command, "move") == 0)
        {
            char *move = strtok(NULL, " ");
            int move_idx = atoi(move);
            int client_id = atoi(client_name);
            sprintf(buffer, "move %d", move_idx);
            sendto(clients_fds[client_id][2], buffer, MAX_MSG_LENGTH, 0, &clients_addrs[clients_fds[client_id][3]], sizeof(struct sockaddr));
        }
        if (strcmp(command, "quit") == 0)
        {
            int client_id = atoi(client_name);
            remove_client(client_id);
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

    for (int i = 0 ; i < MAX_CLIENTS ; i++)
    {
        free(clients_names[i]);
    }
    free(clients_names);

    close(local_socket);
    close(network_socket);

    printf("ENDING\n");
    return 0;
}