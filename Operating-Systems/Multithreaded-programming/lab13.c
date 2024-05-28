#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#define LINES_COUNT 10

pthread_cond_t condVar;
pthread_mutex_t mutex;
int parentCanPrint = 1;

int check_error (int code) {
    if (code != 0) {
        char error[256];
        strerror_r(code, error, sizeof(error));
        fprintf(stderr, "error: %d - %s\n", code, error);
        return 1;
    }
    else return 0;
}

void printParent() {
	for (int i = 0; i < LINES_COUNT; i++) {
		pthread_mutex_lock(&mutex);
		if (parentCanPrint == 0)
			pthread_cond_wait(&condVar, &mutex);
		printf("Parent\n");
		parentCanPrint = 0;
		pthread_cond_signal(&condVar);
		pthread_mutex_unlock(&mutex);
	}
}

void* printChild(void* arg) {
	for (int i = 0; i < LINES_COUNT; i++) {
		pthread_mutex_lock(&mutex);
		if (parentCanPrint == 1)
			pthread_cond_wait(&condVar, &mutex);
		printf("Child\n");
		parentCanPrint = 1;
		pthread_cond_signal(&condVar);
		pthread_mutex_unlock(&mutex);
	}
	pthread_exit(NULL);
}

int main(int argc, char* argv[]){
	pthread_t thread;

	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&condVar, NULL);
	printf("%d\n", sizeof(condVar));

	if (check_error(pthread_create(&thread, NULL, printChild, NULL)))
        return 1;
	printParent();

	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&condVar);
	pthread_exit(NULL);
}
