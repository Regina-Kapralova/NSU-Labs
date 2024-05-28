#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>
#include <string.h>

sem_t parent, child;

int check_error (int code) {
    if (code != 0) {
        char error[256];
        strerror_r(code, error, sizeof(error));
        fprintf(stderr, "error: %d - %s\n", code, error);
        return 1;
    }
    else return 0;
}

void parent_run() {
	for (int i = 0; i < 10; i++) {
		sem_wait(&parent);
		printf("Hello from parent\n");
		sem_post(&child);
	}
}

void* child_run(void* arg) {
	for (int i = 0; i < 10; i++) {
		sem_wait(&child);
		printf("Hello from child\n");
		sem_post(&parent);
	}
	pthread_exit(NULL);
}

int main(int argc, char* argv[]) {

	pthread_t child_t;
	sem_init(&parent, 0, 1);
	sem_init(&child, 0, 0);

	if (check_error(pthread_create(&child_t, NULL, child_run, NULL)))
		return 1;
	parent_run();

	sem_destroy(&parent);
	sem_destroy(&child);
	pthread_exit(NULL);
}
