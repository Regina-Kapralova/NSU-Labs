#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

void finishing (void* param) {
    printf("child has to finish\n");
}

void* run(void* param){
    pthread_cleanup_push(finishing, NULL);
    while (1) {
        printf("this line from child\n");
        pthread_testcancel();
    }
    pthread_cleanup_pop(0);
}

int check_error (int code) {
    if (code != 0) {
        char error[256];
        strerror_r(code, error, sizeof(error));
        fprintf(stderr, "error: %d - %s\n", code, error);
        return 1;
    }
    else return 0;
}

int main(int argc, char* argv[]) {
    pthread_t child;
    if (check_error(pthread_create(&child, NULL, run, NULL)))
        exit (1);
    sleep(2);
    if (check_error(pthread_cancel(child)))
        exit(1);
    pthread_exit(NULL);
    exit(0);
}
