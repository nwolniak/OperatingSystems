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

#define MAX_MESSAGE_LENGTH 256
#define WAIT 0
#define START 1
#define OPPONENT_MOVE 2
#define WAIT_OPPONENT_MOVE 3
#define MOVE 4
#define QUIT 5

int server_socket;
int client_id = -1;
int sign;
char* command;
char buffer[MAX_MESSAGE_LENGTH + 1];
char *name;
int board[9];
int state;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void sig_handler(int signo)
{
    char buffer[MAX_MESSAGE_LENGTH + 1];
    sprintf(buffer, "quit: :%s", name);
    send(server_socket, buffer, MAX_MESSAGE_LENGTH, 0);
    close(server_socket);
    exit(0);
}

void print_board()
{
    for (int i = 0 ; i < 3 ; i++)
    {
        for (int j = 0 ; j < 3 ; j++)
        {
            if (board[i*3 + j] == -1)
            {
                printf(" - ");
            }
            else if (board[i*3 + j] == 0)
            {
                printf(" O ");
            }
            else if (board[i*3 + j] == 1)
            {
                printf(" X ");
            }
        }
        printf("\n");
    }
}

int who_is_winner()
{
    int field_sign = -1;

    /* HORIZONTAL */
    for (int i = 0 ; i < 3 ; i++)
    {
        field_sign = board[i*3];
        if (field_sign == -1) continue;
        for (int j = 1 ; j < 3 ; j++)
        {
            if (board[i*3 + j] != field_sign)
            {
                field_sign = -1;
                break;
            }
        }
        if (field_sign != -1)
        {
            return field_sign;
        }
    }

    /* VERTICAL */
    for (int i = 0 ; i < 3 ; i++)
    {
        field_sign = board[i];
        if (field_sign == -1) continue;
        for (int j = i ; j < i + 7 ; j=j+3)
        {
            if (board[j] != field_sign)
            {
                field_sign = -1;
                break;
            }
        }
        if (field_sign != -1)
        {
            return field_sign;
        }
    }

    /* CROSS */
    field_sign = board[0];
    if (field_sign != -1)
    {
        for (int i = 0 ; i < 9 ; i=i+4)
        {
            if (board[i] != field_sign)
            {
                field_sign = -1;
                break;
            }
        }
    }
    if (field_sign != -1) return field_sign;
    
    field_sign = board[2];
    if (field_sign != -1)
    {
        for (int i = 2 ; i < 7 ; i=i+2)
        {
            if (board[i] != field_sign)
            {
                field_sign = -1;
                break;
            }
        }
    }
    if (field_sign != -1) return field_sign;

    return -1;
}

int is_draw()
{
    for (int i = 0 ; i < 9 ; i++)
    {
        if (board[i] == -1) return 0;
    }
    return 1;
}

void check_board()
{
    int winner = who_is_winner();
    if (winner == -1)
    {
        if (is_draw())
        {
            printf("Its draw!\n");
            state = QUIT;
        }
    }
    else if (winner == sign)
    {
        printf("%s %s\n", name, "won O/X game");
        state = QUIT;
    }
    else
    {
        printf("%s %s\n", name, "lost O/X game");
        state = QUIT;
    }

    if (!is_draw())
    {
        if (state == OPPONENT_MOVE)
        {
            state = MOVE;
        }
        else if (state == MOVE)
        {
            state = WAIT_OPPONENT_MOVE;
        }
    }
}


void* xo_game()
{
    while (1)
    {
        if (state == WAIT)
        {
            /* CHECK COMMAND */
            if (strcmp(command, "invalid_name") == 0)
            {
                printf("Name is taken or invalid\n");
                exit(1);
            }
            printf("Waiting for opponent\n");
            pthread_mutex_lock(&mutex);
            while (state != START)
            {
                pthread_cond_wait(&cond, &mutex);
            }
            pthread_mutex_unlock(&mutex);
        }
        else if (state == START)
        {
            /* SET SIGN */
            if (strcmp(command, "O") == 0)
            {
                sign = 0;
            }
            else if (strcmp(command, "X") == 0)
            {
                sign = 1;
            }
            
            if (sign == 0)
            {
                state = MOVE;
            }
            else if (sign == 1)
            {
                state = WAIT_OPPONENT_MOVE;
            }
            printf("sign %d\n", sign);
            /* INIT GAME BOARD */
            for (int i = 0 ; i < 9 ; i++)
            {
                board[i] = -1;
            }            
        }
        else if (state == WAIT_OPPONENT_MOVE)
        {
            printf("Waiting for opponent move\n");
            pthread_mutex_lock(&mutex);
            while (state != OPPONENT_MOVE)
            {
                pthread_cond_wait(&cond, &mutex);
            }
            pthread_mutex_unlock(&mutex);
        }
        else if (state == OPPONENT_MOVE)
        {
            int move_idx = atoi(command);
            printf("Opponent move %d\n", move_idx);
            if (move_idx >= 0 && move_idx < 9 && board[move_idx] == -1)
            {
                board[move_idx] = (sign + 1) % 2; // opponent sign
            }
            check_board();
        }
        else if (state == MOVE)
        {
            print_board();
            int move_idx;
            scanf("%d", &move_idx);
            board[move_idx] = sign;
            char move_msg[MAX_MESSAGE_LENGTH];
            sprintf(move_msg, "move %d %d", client_id, move_idx);
            send(server_socket, move_msg, MAX_MESSAGE_LENGTH, 0);

            check_board();
        }
        else if (state == QUIT)
        {
            printf("ENDING\n");
            char quit_message[MAX_MESSAGE_LENGTH + 1];
            sprintf(quit_message, "quit %d", client_id);
            send(server_socket, buffer, MAX_MESSAGE_LENGTH, 0);
            client_id = -1;
            exit(0);
        }
    }
}

int main(int argc, char *argv[])
{
    /* SIGNAL HANDLER */
    signal(SIGINT, sig_handler);

    /* ARGS */
    if (argc != 4)
    {
        printf("Invalid number of arguments!\n");
        return 1;
    }
    name = argv[1];
    char *type = argv[2];
    char *addr = argv[3];

    /* CONNECT TO SERVER */
    if (strcmp(type, "local") == 0)
    {
        server_socket = socket(AF_UNIX, SOCK_STREAM, 0);
        struct sockaddr_un local_sockaddr;
        memset(&local_sockaddr, 0, sizeof(struct sockaddr_un));
        local_sockaddr.sun_family = AF_UNIX;
        strcpy(local_sockaddr.sun_path, addr);
        connect(server_socket, (struct sockaddr *)&local_sockaddr,
                sizeof(struct sockaddr_un));
    }
    
    /* SEND MESSAGE TO SERVER */
    char buffer[MAX_MESSAGE_LENGTH];
    sprintf(buffer, "register %s", name);
    if (strcmp(type, "local") == 0)
    {
        send(server_socket, buffer, MAX_MESSAGE_LENGTH, 0);
    }
    

    
    char* token;
    pthread_t game_thread;
    while (1)
    {
        recv(server_socket, buffer, MAX_MESSAGE_LENGTH, 0);
        token = strtok(buffer, " ");
        command = strtok(NULL, " ");

        pthread_mutex_lock(&mutex);
        
        if (strcmp(token, "wait") == 0)
        {
            state = WAIT;
            client_id = atoi(command); // id from server
            printf("client id %d\n", client_id);
            pthread_create(&game_thread, NULL, xo_game, NULL);
        }

        else if (strcmp(token, "start") == 0)
        {
            state = START;
            
        }
        else if (strcmp(token, "move") == 0)
        {
            printf("Opponent moved!\n");
            state = OPPONENT_MOVE;
        }
        else if (strcmp(token, "quit") == 0)
        {
            state = QUIT;
            pthread_join(game_thread, NULL);
        }
        else if (strcmp(token, "ping") == 0)
        {
            sprintf(buffer, "ping %d", client_id);
            send(server_socket, buffer, MAX_MESSAGE_LENGTH, 0);
        }
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }
    return 0;
}