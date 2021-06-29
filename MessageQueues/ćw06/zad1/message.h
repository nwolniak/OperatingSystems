#define TEXT_SIZE 512
#define STOP 1
#define DISCONNECT 2
#define LIST 3
#define INIT 4
#define CONNECT 5
#define CHAT 6

typedef struct mymsg
{
	long int type; /* message type */
	char text[TEXT_SIZE]; /* message text */
} mymsg;

#define MAX_MSG_SIZE sizeof(mymsg) - sizeof(long int)

void send_message(int msq_id, mymsg* message);
void receive_message(int msq_id, mymsg* message, int type);
int receive_message_nowait(int msq_id, mymsg* message, int type);