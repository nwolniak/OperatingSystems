#include <unistd.h> 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>


int main(int argc, char* argv[]) {
	if (argc != 4){
		printf("Invalid number of arguments!\n");
		return 1;
	}

	char* pipe_file = calloc(strlen(argv[1]), sizeof(char));
	strcpy(pipe_file, argv[1]);
	char* file_write = calloc(strlen(argv[2]), sizeof(char));
	strcpy(file_write, argv[2]);
	int N = atoi(argv[3]);


	/* OPEN FILE TO WRITE */
	FILE * f_write;
	if ((f_write = fopen(file_write, "w")) == NULL){
		printf("Could not open the file!\n");
		puts(strerror(errno));
		return 1;
	}

	/* OPEN FIFO PIPE */
	FILE* p_file;
	if ((p_file = fopen(pipe_file, "r")) == NULL){
		printf("Could not open the fifo pipe!\n");
		puts(strerror(errno));
		return 1;
	}

	char** new_file = calloc(4, sizeof(char*));
	for (int i = 0 ; i < 10 ; i ++){
		new_file[i] = calloc(100, sizeof(char));
	}


	/* READ FROM PIPE */
	char* buffer = calloc(N, sizeof(char));
	int read = 0;
	while((read = fread(buffer, sizeof(char), N, p_file)) > 0) {
		/*
	
		printf("%s", buffer);
	
		*/


		char* buff_c = calloc(N, sizeof(char));
		memcpy(buff_c, buffer, N);
		char* token = strtok(buff_c, " \n");
		int row = atoi(token);
		token = strtok(NULL, " ");

		char* token_tmp = calloc(strlen(token), sizeof(char));
		memcpy(token_tmp, token, strlen(token));
		memcpy(new_file[row], strcat(new_file[row], token_tmp), 100);

		free(token_tmp);
		free(buff_c);
		free(buffer);
		buffer = calloc(N, sizeof(char));
	}

	for(int i = 0 ; i < 4 ; i ++){
		fwrite(new_file[i], sizeof(char), sizeof(new_file[i]), f_write);
		fflush(f_write);
	}

	free(pipe_file);
	free(file_write);
	free(buffer);
	fclose(f_write);
	fclose(p_file);

	return 0;
}
