#include <unistd.h> 
#include <fcntl.h>  
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>


typedef struct {
    char **args;
    int size;
} Commands;

typedef struct {
    Commands** pipes;
    Commands** arguments;
    int pipes_size;
    int arguments_size;
} Main_table;


char* get_function_name(char * line){
	char* line_c = calloc(strlen(line), sizeof(char));
	memcpy(line_c, line, strlen(line));
	char* token = strtok(line_c, " ");
	printf("%s\n", token);
	return token;
}


char** get_args(char * line){
	char** args = calloc(0, sizeof(char*));
	char* line_c = calloc(strlen(line), sizeof(char));
	memcpy(line_c, line, strlen(line));

    char *token = NULL;
    int i = 0;
    for (token = strtok(line_c, " ");
         token != NULL;
         token = strtok(NULL, " ")) { 
    	args = realloc(args, sizeof(char*) * (i+1));
    	args[i] = calloc(strlen(token), sizeof(char));
    	memcpy(args[i], token, strlen(token));
        i++;
    }
    free(line_c);
	return args;
}

int get_args_len(char *line){
    char *token = NULL;
    char* line_c = calloc(strlen(line), sizeof(char));
	memcpy(line_c, line, strlen(line));
    int i = 0;
    for (token = strtok(line_c, " ");
         token != NULL;
         token = strtok(NULL, " ")) {   
        i++;
    }
    free(line_c);
    return i;
}

Main_table create_table() {
    Commands** pipes = calloc(0, sizeof(Commands*));
	Commands** arguments = calloc(0, sizeof(Commands*));
    Main_table main_table;
    main_table.pipes = pipes;
    main_table.arguments = arguments;
    main_table.pipes_size = 0;
    main_table.arguments_size = 0;
    return main_table;
}

void parse_file(Main_table* main_table, char* file_path) {
	/* OPEN FILE */
	FILE* f;
	if ((f = fopen(file_path, "r")) == NULL ){
		printf("Could not open the file!\n");
		exit(1);
	}

    /* READ EXPRESSIONS AND SAVE TO MAIN TABLE  */
    char * buffer = NULL;
	size_t buff_size = 0;
	char* line;
	while (((getline(&buffer, &buff_size, f)) != -1)) {
		if (strcmp(buffer, "\n") == 0) break;
		line = strtok(buffer, "=\n");
		line = strtok(NULL, "=\n");
		
		char ** pipe_args = calloc(0, sizeof(char*));
		int size = 0;
		char* expression = strtok(line, "|");
		do{
			if (expression[strlen(expression) - 1] == '\n'){
				expression[strlen(expression) - 1] = '\0';
			}
			pipe_args = realloc(pipe_args, sizeof(char*) * (++size));
			pipe_args[size-1] = calloc(strlen(expression),sizeof(char));
			memcpy(pipe_args[size-1], expression, strlen(expression));
		}while((expression = strtok(NULL, "|")) != NULL);

		Commands *pipes = calloc(1, sizeof(Commands));
		pipes->args = pipe_args;
		pipes->size = size;
		main_table->pipes = realloc(main_table->pipes, sizeof(Commands*) * (main_table->pipes_size + 1));
		main_table->pipes[main_table->pipes_size] = pipes;
		main_table->pipes_size++;
	}

	/* READ ORDERS AND SAVE TO MAIN TABLE */
	while (((getline(&buffer, &buff_size, f)) != -1)) {
		char ** argument_args = calloc(0, sizeof(char*));
		int size = 0;
		char* expression = strtok(buffer, " |");
		do{
			if (expression[strlen(expression) - 1] == '\n'){
				expression[strlen(expression) - 1] = '\0';
			}
			argument_args = realloc(argument_args, sizeof(char*) * (++size));
			argument_args[size-1] = calloc(strlen(expression) - 8,sizeof(char));
			memcpy(argument_args[size-1], expression+9, strlen(expression) - 9);
		}while((expression = strtok(NULL, " |")) != NULL);

		Commands *arguments = calloc(1, sizeof(Commands));
		arguments->args = argument_args;
		arguments->size = size;
		main_table->arguments = realloc(main_table->arguments, sizeof(Commands*) * (main_table->arguments_size + 1));
		main_table->arguments[main_table->arguments_size] = arguments;
		main_table->arguments_size++;

	}
	free(buffer);
	fclose(f);
}



void remove_table(Main_table* main_table){
	for (int i = main_table->pipes_size - 1 ; i >= 0 ; i--){
		for (int j = main_table->pipes[i]->size -1; j >= 0 ; j--){
			free(main_table->pipes[i]->args[j]);
		}
		free(main_table->pipes[i]);
	}
	for (int i = main_table->arguments_size - 1 ; i >= 0 ; i--){
		for (int j = main_table->arguments[i]->size -1; j >= 0 ; j--){
			free(main_table->arguments[i]->args[j]);
		}
		free(main_table->arguments[i]);
	}
}

int main(int argc, char* argv[]){
	if (argc != 2){
		printf("Invalid number of arguments!\n");
		return 1;
	}

	/* PARSE FILE */
	Main_table main_table = create_table();
	parse_file(&main_table, argv[1]);


	//char tmp[] = " cat /etc/passwd ";
	//get_args(tmp);

	/*
	for(int i = 0 ; i < main_table.pipes_size ; i++){
		for (int j = 0 ; j < main_table.pipes[i]->size; j++){
			char * expression = main_table.pipes[i]->args[j];
			printf("%s\n", expression);
		}
	}


	for(int i = 0 ; i < main_table.arguments_size ; i++){
		for (int j = 0 ; j < main_table.arguments[i]->size; j++){
			printf("%s\n", main_table.arguments[i]->args[j]);
		}
	}
	*/

	
	
	for (int i = 0 ; i < main_table.arguments_size ; i++){

		int number_of_pipes = 0;
		for (int j = 0 ; j < main_table.arguments[i]->size ; j++){
			int pipe_index = atoi(main_table.arguments[i]->args[j]) -1;
			for (int k = 0 ; k < main_table.pipes[pipe_index]->size ; k++){
				number_of_pipes++;
			}
		}

		int fd[number_of_pipes][2];
		for (int j = 0 ; j < number_of_pipes; j++){
			if (pipe(fd[j]) < 0){
				printf("Fail in pipe()\n");
				return 1;
			}
		}

		for (int j = 0 ; j < main_table.arguments[i]->size ; j++){
			int pipe_index = atoi(main_table.arguments[i]->args[j]) -1;
			for (int k = 0 ; k < main_table.pipes[pipe_index]->size; k++){
				char* expression = main_table.pipes[pipe_index]->args[k];
				pid_t child;
				if ((child = fork()) < 0){
					printf("Fail in fork()\n");
					puts(strerror(errno));
					return 1;
				} 
				else if (child == 0){
					if (j + k > 0)
		            {
		                dup2(fd[j + k - 1][0], STDIN_FILENO); // 0 odczyt
		            }
		            if (j + k < number_of_pipes -1)
		            {
		                dup2(fd[j + k][1], STDOUT_FILENO); // 1 zapis
		            }
		            
		            for (int q = 0; q < number_of_pipes - 1; q++)
		            {
		                close(fd[q][0]);
		                close(fd[q][1]);
		            }


		            if (execvp(get_function_name(expression),get_args(expression)) == -1){
		            	printf("Fail in execl\n");
						puts(strerror(errno));
						exit(1);
		            }
		            exit(0);
				}else{
					//wait(0);
				}
			}
		}
		for (int j = 0 ; j < number_of_pipes; j++){
			close(fd[j][0]);
			close(fd[j][1]);
		}
		for (int i = 0; i < number_of_pipes; ++i)
	    {
	        wait(0);
	    }
	}
	
	
	remove_table(&main_table);
	return 0;
}
