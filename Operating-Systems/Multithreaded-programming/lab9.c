#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#define iters_count 1000;

int threads_count;
int isSig = 0;
int isExit = 0;
int count = 0;

pthread_barrier_t barrier;
pthread_mutex_t mutex;

void func(int sig) {
	isSig = 1;
}
struct param {
    double result;
    int rank;
}typedef params;

int check_error (int code) {
    if (code != 0) {
        char error[256];
        strerror_r(code, error, sizeof(error));
        fprintf(stderr, "error: %d - %s\n", code, error);
        return 1;
    }
    else return 0;
}

void* run(void *param) {
	double local_pi = 0.0;
	int i = ((params*)param)->rank;
	long long  check = 0;
	while(1) {
		check += iters_count;
		for (; i < check; i += threads_count) {
            local_pi += 1.0 / (i * 4.0 + 1.0);
            local_pi -= 1.0 / (i * 4.0 + 3.0);
		}
		pthread_mutex_lock(&mutex);
		if (++count == threads_count) {
			count = 0;
			if (isSig)
				isExit = 1;
		}
		pthread_mutex_unlock(&mutex);

        pthread_barrier_wait(&barrier);
        if (isExit)
			break;
	}
	((params*)param)->result = local_pi;
	pthread_exit(NULL);
}

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Please, enter count of threads.\n");
        return 0;
    }
    threads_count = atoi(argv[1]);
    if (threads_count < 1) {
        printf("Please, enter positive count of threads.\n");
        return 0;
    }

    if (pthread_mutex_init(&mutex, NULL) != 0)
        return 1;
    if (pthread_barrier_init(&barrier, NULL, threads_count) != 0)
        return 1;
	signal(SIGINT, func);

	pthread_t* threads = malloc (sizeof(pthread_t) * threads_count);
	params* param = malloc (sizeof(params) * threads_count);
    double pi = 0.0;

	for (int i = 0; i < threads_count; i++) {
        param[i].rank = i;
		if (check_error(pthread_create(&threads[i], NULL, run, &param[i]))) {
			free(threads);
            free (param);
			exit(1);
		}
	}
	for (int i = 0; i < threads_count; i++) {
		if (check_error(pthread_join(threads[i], NULL))){
			free(threads);
			free (param);
			exit(1);
		}
		pi += param[i].result;
	}

	pi *= 4.0;
	printf ("  pi = %.16f\n", pi);

    free(threads);
    free(param);

    if (pthread_mutex_destroy(&mutex) != 0)
        return 1;
	if (pthread_barrier_destroy(&barrier) != 0)
        return 1;

    pthread_exit(NULL);
}



