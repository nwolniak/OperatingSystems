#include <unistd.h> 
#include <fcntl.h>  
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>


int main(int argc, char* argv[]){
	if (argc != 2 && argc != 4){
		printf("Invalid number of arguments!\n");
		return 1;
	}

	if (argc == 2){
		//char* command = argv[1];
		FILE* f;


		if ((f = popen("mail", "w")) == NULL){
			printf("Fail in popen()\n");
			puts(strerror(errno));
			return 1;
		}
		char echo[] = "echo q";
		fputs(echo, f);


		pclose(f);
	} 
	else
	{
		char* email = argv[1];
		char* title = argv[2];
		char* content = argv[3];
		FILE* f1;
		char* mail_command = calloc(256, sizeof(char));
		strcat(mail_command, "mail -s ");
		strcat(mail_command, title);
		strcat(mail_command, " ");
		strcat(mail_command, email);


		printf("%s\n", mail_command);

		

		if ((f1 = popen(mail_command, "w")) == NULL){
			printf("Fail in popen()\n");
			puts(strerror(errno));
			return 1;
		}

		fputs(content, f1);

		free(mail_command);
		pclose(f1);
	}

	return 0;
}