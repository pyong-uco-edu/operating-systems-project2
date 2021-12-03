#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string>
// #include <semaphore.h>

#include "defs.h"

// void* operations(void* arg) {

// }

int main(int argc, char **argv) {
    if (argc != 2) {
		printf("Usage: %s loop_count\n", argv[0]);
		exit(1);
	}

	int loop_count = atoi(argv[1]);
	if (loop_count == 0) {
		printf("Usage: %s max_loop_count\n", argv[0]);
		exit(1);
	}

    pthread_t threads[10];

    parameters* params[10];

	for (int i = 0; i < 10; i++) {
		params[i] = new parameters;
		params[i]->loop_count = loop_count;
		char* filename = (char*)malloc(sizeof(char) * 18);
		filename = (char *) "race_thread_ .txt\0";
		filename[12] = i;
		// printf("%s\n", filename);
		// params[i]->filename = filename;
		// params[i]->index = i;
	}

    // for (int i = 0; i < 10; i++)
    //     pthread_create(&threads[i], NULL, operations, (void *));
}