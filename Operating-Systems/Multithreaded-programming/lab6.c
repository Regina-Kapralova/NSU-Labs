#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int check_error (int code) {
    if (code != 0) {
        char error[256];
        strerror_r(code, error, sizeof(error));
        fprintf(stderr, "error: %d - %s\n", code, error);
        return 1;
    }
    else return 0;
}

void* run(void* param){
	usleep(strlen((char*)param) * 100000);
	printf("%s", (char*)param);
	pthread_exit(NULL);
}

int main(int argc, char* argv[]){

	if (argc < 2) {
        printf("Please, enter file's name.\n");
		return 0;
    }
    FILE* file;
    if ((file = fopen(argv[1], "r")) == NULL){
        printf("Please, enter correct file's name.\n", argv[1]);
        return 0;
    }

	char strings[256][100];
	int str_count = 0;
	while ((fgets(strings[str_count], 64, file) != NULL) && str_count < 100)
		str_count++;


	pthread_t* threads = malloc (sizeof(pthread_t) * str_count);
	for (int i = 0; i < str_count; i++) {
		if (check_error(pthread_create(&threads[i], NULL, run, (void*)strings[i])) == 1) {
			free(threads);
			exit(1);
		}
	}
	for (int i = 0; i < str_count; i++) {
		if (check_error(pthread_join(threads[i], NULL))){
			free(threads);
			exit(1);
		}
	}
	free(threads);
	pthread_exit(NULL);
}
