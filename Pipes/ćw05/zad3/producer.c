#include <unistd.h> 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>


int main(int argc, char* argv[]) {
	if (argc != 5){
	    printf("Invalid number of arguments!\n");
	    return 1;
	}

	srand(time(NULL));
	char* pipe_file = calloc(strlen(argv[1]), sizeof(char));
	strcpy(pipe_file, argv[1]);
	char* row = calloc(strlen(argv[2]), sizeof(char));
	strcpy(row, argv[2]);
	char* file_read = calloc(strlen(argv[3]), sizeof(char));
	strcpy(file_read, argv[3]);
	int N = atoi(argv[4]);

	/* OPEN FILE TO READ FROM */
	FILE* f_read;
	if ((f_read = fopen(file_read, "r")) == NULL){
	    printf("Could not open the file!\n");
	    puts(strerror(errno));
	    return 1;
	}

	/* OPEN FIFO PIPE TO WRITE IN */
	FILE* p_file;
	if ((p_file = fopen(pipe_file, "w")) == NULL){
	    printf("Could not open the fifo pipe!\n");
	    puts(strerror(errno));
	    return 1;
	}

	/* READ FROM FILE AND WRITE TO FIFO PIPE */
	char* buffer = calloc(N, sizeof(char));
	int read = 0;
	while((read = fread(buffer, sizeof(char), N, f_read)) > 0) {
		sleep(rand() % 2 + 1);
		char* line_to_write = calloc(read + strlen(row) + 1, sizeof(char));
		for(int i = 0 ; i < strlen(row) ; i++) line_to_write[i] = row[i];
		line_to_write[strlen(row)] = ' ';
		for(int i = 0 ; i < read ; i ++) line_to_write[i + strlen(row) + 1] = buffer[i];
		line_to_write[read + strlen(row) + 1] = '\n';

		fwrite(line_to_write, sizeof(char), strlen(line_to_write), p_file);
		fflush(p_file);

		free(line_to_write);
		free(buffer);
		buffer = calloc(N, sizeof(char));
    }

    
	free(buffer);
	free(file_read);
	fclose(f_read);
	fclose(p_file);
	free(pipe_file);

	return 0;
}
