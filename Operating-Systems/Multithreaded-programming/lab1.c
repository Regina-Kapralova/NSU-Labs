#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

void* run(void *params){

    for(int i = 0; i < 10; i++)
        printf("this line from child\n");
    pthread_exit(NULL);
}

int main(int argc, char *argv[]){
    pthread_t child;
    int code = pthread_create(&child, NULL, run, NULL);

    if(code != 0){
        char error[256];
        strerror_r(code, error, sizeof(error));
        fprintf(stderr, "error: %d - %s\n", code, error);
        exit(1);
    }

    for(int i = 0; i < 10; i++)
        printf("this line from parent\n");

    pthread_exit(NULL);
    exit(0);
}

