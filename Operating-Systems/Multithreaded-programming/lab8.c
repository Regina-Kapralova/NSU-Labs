#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define steps_count 200000000

int threads_count;

struct param {
    int rank;
    double result;
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

void* run (void *param) {
	double local_pi = 0.0;
	for (int i = ((params*)param)->rank; i < steps_count ; i += threads_count) {
		local_pi += 1.0 / (i * 4.0 + 1.0);
		local_pi -= 1.0 / (i * 4.0 + 3.0);
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
	printf ("pi = %.16f\n", pi);

    free(threads);
    free (param);
    pthread_exit(NULL);
}



