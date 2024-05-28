#include <pthread.h>
#include  <stdio.h>
#include <semaphore.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

sem_t isEmpty;
sem_t isFull;
sem_t mutex;

int Exit = 0;
void finish(int sig) {
    Exit = 1;
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

typedef struct Message {
	char message[81];
	struct Message* next;
	struct Message* prev;
} Message;

typedef struct Queue {
	Message* head;
	Message* tail;
	int isDroped;
} Queue;

void mymsginit(Queue* queue) {
	sem_init(&isEmpty, 0, 10);
	sem_init(&isFull, 0, 0);
	sem_init(&mutex, 0, 1);
	queue->head = NULL;
	queue->tail = NULL;
	queue->isDroped = 0;
}

void mymsqdrop(Queue* queue) {
	sem_wait(&mutex);
	queue->isDroped = 1;
	sem_post(&isEmpty);
	sem_post(&isFull);
	sem_post(&mutex);
}

void mymsgdestroy(Queue* queue) {
	sem_wait(&mutex);
	Message* tmp = queue->head;
	Message* del_tmp;
	while (tmp) {
		del_tmp = tmp;
		tmp = tmp->next;
		free(del_tmp);
	}
	sem_post(&mutex);
	sem_destroy(&mutex);
	sem_destroy(&isEmpty);
	sem_destroy(&isFull);
}

int mymsgget(Queue* queue, char* buf, int bufSize) {
    if (queue == NULL)
        return 0;
	sem_wait(&isFull);
	sem_wait(&mutex);
	if (queue->isDroped == 1) {
		sem_post(&mutex);
		sem_post(&isFull);
		return 0;
	}
	Message* res = queue->tail;
	if (queue->head == queue->tail) {
		queue->head = NULL;
		queue->tail = NULL;
	}
	else {
		queue->tail = queue->tail->prev;
		queue->tail->next = NULL;
	}
	strncpy(buf, res->message, bufSize);
	sem_post(&mutex);
	sem_post(&isEmpty);
	return bufSize;
}

int mymsgput(Queue* queue, char* msg) {
    if (queue == NULL)
        return 0;
	sem_wait(&isEmpty);
	sem_wait(&mutex);
	if (queue->isDroped == 1) {
		sem_post(&mutex);
		sem_post(&isEmpty);
		return 0;
	}
	Message* new_mes;
	new_mes = (Message*)malloc(sizeof(Message));
	new_mes->prev = NULL;
	new_mes->next = NULL;
	Message* tmp;
	sprintf(new_mes->message, "%s", "");
	strncat(new_mes->message, msg, 80);
    printf("%s\n", new_mes->message);
	tmp = queue->head;
	queue->head = new_mes;
	if (tmp != NULL) {
		(queue->head)->next = tmp;
		tmp->prev = queue->head;
	}
	if (tmp == NULL)
		queue->tail = new_mes;
	sem_post(&mutex);
	sem_post(&isFull);
    return strlen(msg);
}

void* producer(void* q) {
    Queue* queue = (Queue*)q;
    for (int i = 0; i < 100; i++) {
        char buf[40];
        sprintf(buf, "Produce msg %d from %lu", i, pthread_self());
        if (!mymsgput(queue, buf))
            break;
    }
    pthread_exit(NULL);
}

void* consumer(void* q) {
    Queue* queue = (Queue*)q;
    while (1) {
        char buf[41];
        if (mymsgget(queue, buf, sizeof(buf)))
            printf("Consumer received from %lu: %s\n", pthread_self(), buf);
        else
            break;
    }
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    Queue queue;
    int producers_count, consumers_count;

    if (argc < 2) {
        printf("Please enter count of procedures and consumers.\n");
        pthread_exit(NULL);
    }
    producers_count = atoi(argv[1]);
    consumers_count = atoi(argv[2]);

    pthread_t *producers, *consumers;
    producers = (pthread_t*)malloc(sizeof(pthread_t) * producers_count);
    consumers = (pthread_t*)malloc(sizeof(pthread_t) * consumers_count);

    signal(SIGINT, finish);
    mymsginit(&queue);

	for (int i = 0; i < producers_count; i++) {
		if (check_error(pthread_create(&producers[i], NULL, producer, &queue))) {
			free(producers);
			exit(1);
		}
	}
	for (int i = 0; i < consumers_count; i++) {
		if (check_error(pthread_create(&consumers[i], NULL, consumer, &queue))) {
			free(consumers);
			exit(1);
		}
	}
    while (!Exit)
        pause();
    mymsqdrop(&queue);

	for (int i = 0; i < producers_count; i++) {
        if (check_error(pthread_join(producers[i], NULL))) {
			free(producers);
			exit(1);
		}
	}
    for (int i = 0; i < consumers_count; i++) {
        if (check_error(pthread_join(consumers[i], NULL))) {
			free(consumers);
			exit(1);
		}
	}

    mymsgdestroy(&queue);
    printf("\nExiting...\n");
    free(producers);
    free(consumers);
    pthread_exit(NULL);
}
