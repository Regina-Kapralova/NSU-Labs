#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

void* run (void* param) {
    char** msg = (char**)param;
    for (int i = 0; i < 4; i++)
        printf("%s  ", msg[i]);
    pthread_exit(NULL);
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

    pthread_t child1, child2, child3, child4;

    char* msg1[4] = {"from", "first", "child", ";"};
    char* msg2[4] = {"from", "second", "child", ";"};
    char* msg3[4] = {"from", "third", "child", ";"};
    char* msg4[4] = {"from", "fourth", "child1", ";"};

    if (check_error(pthread_create(&child1, NULL, run, &msg1)))
        exit(1);
    if (check_error(pthread_create(&child2, NULL, run, &msg2)))
        exit(1);
    if (check_error(pthread_create(&child3, NULL, run, &msg3)))
        exit(1);
    if (check_error(pthread_create(&child4, NULL, run, &msg4)))
        exit(1);

    pthread_exit(NULL);
    exit(0);
}

