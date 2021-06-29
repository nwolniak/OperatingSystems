#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdint.h>
#include <sys/time.h>

typedef struct {
    int **arr;
} img_arr;

uint64_t get_time() {
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return tv.tv_sec*(uint64_t)1000000+tv.tv_usec;
}


int width, height, max_value;
int threads_n;
img_arr* images;
img_arr img_out;
img_arr img_block;

img_arr create_img_arr(int width, int height) {
	int** arr = calloc(height, sizeof(int*));
	for (int i = 0 ; i < height ; i++)
	{
		arr[i] = calloc(width, sizeof(int));
	}

	img_arr img;
	img.arr = arr;
	return img;
}
void remove_img_arr(img_arr* img){
	for (int i = height - 1 ; i >= 0 ; i--){
		free(img->arr[i]);
	}
}


img_arr load_img(char* img_name)
{
	FILE* img_fd;
	if ((img_fd = fopen(img_name, "r")) == NULL)
	{
		perror("Invalid file name");
		exit(1);
	}

	char *buffer = NULL;
	size_t buff_size = 0;
	getline(&buffer, &buff_size, img_fd); // header
	getline(&buffer, &buff_size, img_fd); // comments

	/* WIDTH AND HEIGHT */
	getline(&buffer, &buff_size, img_fd); 
	char* token = strtok(buffer, " \n");
	width = atoi(token);
	token = strtok(NULL, " \n");
	height = atoi(token);

	/* MAX VALUE */
	getline(&buffer, &buff_size, img_fd); 
	token = strtok(buffer, " \n");
	max_value = atoi(token);

	img_arr img = create_img_arr(width, height);

	char* value;
	int i = 0;
	int j = 0;
	while (getline(&buffer, &buff_size, img_fd) > 0)
	{
		value = strtok(buffer, " \n");
		do{
			if ( j == width ) { i++; j = 0;}
			img.arr[i][j++] = atoi(value);
		} while((value = strtok(NULL, " \n")) != NULL);


	}
	fclose(img_fd);
	return img;
}

void save_img(char* img_out_name)
{
	FILE *img_out_fd = fopen(img_out_name, "w");
	fprintf(img_out_fd, "%s\n", "P2");
	fprintf(img_out_fd, "%s\n", "# IMG NEGATED COLORS");
	fprintf(img_out_fd, "%d %d\n", width, height);
	fprintf(img_out_fd, "%d\n", max_value);
	
	for (int i = 0 ; i < height ; i++)
	{
		for (int j = 0 ; j < width ; j++)
		{
			if (j == width - 1)
			{
				fprintf(img_out_fd, "%d\n", img_out.arr[i][j]);
			}
			else
			{
				fprintf(img_out_fd, "%d ", img_out.arr[i][j]);

			}
		}
	}
}

void* numbers(void* n)
{
	uint64_t start = get_time();

	int thread_n = (intptr_t) n;
	int start_value, end_value;
	start_value = thread_n * (int)(max_value + 1) / threads_n;
	end_value = start_value + (int)(max_value + 1) / threads_n;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (start_value <= images[thread_n].arr[i][j] || images[thread_n].arr[i][j] < end_value)
			{
				img_out.arr[i][j] = max_value - images[thread_n].arr[i][j];
			}
		}
	}
	uint64_t delta = get_time() - start;
	int* real_time = calloc(1, sizeof(int));
	*real_time = delta;

	return (void*) real_time;
}

void* block(void* n)
{
	uint64_t start = get_time();

	int thread_n = (intptr_t) n;
	int dx = (int)(width / threads_n);
	for (int i = 0; i < height; i++)
	{
		for (int j = thread_n * dx; j < (thread_n + 1) * dx; j++)
		{
			img_out.arr[i][j] = max_value - img_block.arr[i][j];
		}
	}
	uint64_t delta = get_time() - start;
	int* real_time = calloc(1, sizeof(int));
	*real_time = delta;
		
	return (void*) real_time;
}



int main(int argc, char* argv[])
{
	if (argc != 5)
	{
		printf("Invalid number of arguments!");
		return 1;
	}

	/* ARGUMENTS */
	threads_n = atoi(argv[1]);
	char* command = argv[2];
	char* img_name = argv[3];
	char* img_out_name = argv[4];
		
	/* CREATE COPIES OF IMG */
	if (strcmp(command, "numbers") == 0)
	{
		images = calloc(threads_n, sizeof(img_name));
		images[0] = load_img(img_name);
		for (int i = 1 ; i < threads_n ; i++)
		{
			images[i] = create_img_arr(width, height);

			for (int h = 0 ; h < height ; h++)
			{
				for (int w = 0 ; w < width ; w++)
				{
					memcpy(&images[i].arr[h][w], &images[0].arr[h][w], 3);
				}
			}
		}
	}
	else if (strcmp(command, "block") == 0)
	{
		img_block = load_img(img_name);
	}

	img_out = create_img_arr(width, height);

	/* CREATE THREADS */
	uint64_t start_total = get_time();
	pthread_t *threads = calloc(threads_n, sizeof(pthread_t));
	int* threads_args = calloc(threads_n, sizeof(int));
	for (int i = 0 ; i < threads_n ; i++)
	{
		threads_args[i] = i;
		if (strcmp(command, "numbers") == 0)
		{
			pthread_create(&threads[i], NULL, numbers, (void*) (intptr_t) threads_args[i]);
		}
		else if (strcmp(command, "block") == 0)
		{
			pthread_create(&threads[i], NULL, block, (void*) (intptr_t) threads_args[i]);
		}
		else
		{
			printf("Invalid command\n");
			return 2;
		}
	}

	/* WAIT FOR THREADS */
	int times[threads_n];
	void* retval;
    for (int i = 0 ; i < threads_n ; i++)
	{
		pthread_join(threads[i], &retval);
		int* real_time = (int*) retval;
		times[i] = *real_time;
	}
    uint64_t delta_total = get_time() - start_total;

	save_img(img_out_name);

	FILE* times_file = fopen("Times.txt", "a");
	fprintf(times_file, "%s\n", "#############");
	fprintf(times_file, "%s\n", img_name);
	fprintf(times_file, "COMMAND: %s\n", command);
	fprintf(times_file, "THREADS NUMBER: %d\n", threads_n);
	fprintf(times_file, "WIDTH: %d  HEIGHT: %d\n", width, height);
	for (int i = 0 ; i < threads_n ; i++)
	{
		printf("THREAD: %d TIME: %d μs\n", i+1, times[i]);
		fprintf(times_file, "THREAD: %d TIME: %d μs\n", i+1, times[i]);
	}
	printf("TOTAL TIME: %d μs\n", (int) delta_total);
	fprintf(times_file, "TOTAL TIME: %d μs\n\n\n", (int) delta_total);


	if (strcmp(command, "numbers") == 0)
	{
		for (int i = 0 ; i < threads_n ; i++) {remove_img_arr(&images[i]);}
		free(images);
	}
	else
	{
		remove_img_arr(&img_block);
	}
	
	free(threads_args);
	return 0;
}