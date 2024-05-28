#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <signal.h>

sem_t count_a, count_b, count_ab, count_c;

int Exit = 0;
void finish(int sign) {
    Exit = 1;
}

void* create_a (void* arg){
    int value;
    while(1){
        if (Exit == 1)
            pthread_exit(NULL);
        sleep(1);
        sem_getvalue(&count_a, &value);
        if (value < 5) {
            sem_post(&count_a);
            printf("A created\n");
        }
    }
}

void* create_b (void* arg){
    int value;
    while(1){
        if (Exit == 1)
            pthread_exit(NULL);
        sleep(2);
        sem_getvalue(&count_b, &value);
        if (value < 5) {
            sem_post(&count_b);
            printf("B created\n");
        }
    }
}

void* create_ab (void* arg){
    int value;
    while(1){
        if (Exit == 1)
            pthread_exit(NULL);
        if(value < 5) {
            sem_wait(&count_a);
            sem_wait(&count_b);
            sem_post(&count_ab);
            printf("AB created\n");
        }
    }
}

void* create_c (void* arg){
    int value;
    while(1){
        if (Exit == 1)
            pthread_exit(NULL);
        sleep(3);
        sem_getvalue(&count_c, &value);
        if (value < 5) {
            sem_post(&count_c);
            printf("C created\n");
        }
    }
}

void* create_widget (){
    while(1){
        if (Exit == 1)
            pthread_exit(NULL);
        sem_wait(&count_ab);
        sem_wait(&count_c);
        printf("Widget created!\n");
    }
}

int main(){
    pthread_t masrer_a;
    pthread_t master_b;
    pthread_t master_ab;
    pthread_t master_c;

    sem_init(&count_a, 0, 0);
    sem_init(&count_b, 0, 0);
    sem_init(&count_ab, 0, 0);
    sem_init(&count_c, 0, 0);

    signal(SIGINT, finish);

    pthread_create(&masrer_a, NULL, create_a, NULL);
    pthread_create(&master_b, NULL, create_b, NULL);
    pthread_create(&master_ab, NULL, create_ab, NULL);
    pthread_create(&master_c, NULL, create_c, NULL);
    create_widget();

    pthread_join(masrer_a, NULL);
    pthread_join(master_b, NULL);
    pthread_join(master_ab, NULL);
    pthread_join(master_c, NULL);

    sem_destroy(&count_a);
    sem_destroy(&count_b);
    sem_destroy(&count_ab);
    sem_destroy(&count_c);
}
