#include <pthread.h>
#include  <stdio.h>
#include <semaphore.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

pthread_mutex_t mutex;
pthread_cond_t cond;
int producers_count;
int consumers_count;

int Finished = 0;
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
	int count;
} Queue;

void mymsginit(Queue* queue){
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cond, NULL);
	queue->head = NULL;
	queue->tail = NULL;
	queue->isDroped = 0;
	queue->count = 0;
}

void mymsqdrop(Queue* queue) {
	pthread_mutex_lock(&mutex);
	queue->isDroped = 1;
	pthread_cond_broadcast(&cond);
	pthread_mutex_unlock(&mutex);
}

void mymsgdestroy(struct Queue* queue) {
	Message* tmp = queue->head;
	pthread_mutex_lock(&mutex);
	Message* del_tmp;
	while (tmp) {
		del_tmp = tmp;
		tmp = tmp->next;
		free(del_tmp);
	}
	pthread_mutex_unlock(&mutex);
	pthread_cond_destroy(&cond);
	pthread_mutex_destroy(&mutex);
}

int mymsgget(Queue* queue, char* buf, int bufSize){
	if (queue == NULL)
		return 0;
	pthread_mutex_lock(&mutex);
	while (!( queue->count > 0 || (Finished == producers_count && queue->count == 0) || queue->isDroped != 1))
		pthread_cond_wait(&cond, &mutex);
	if (queue->isDroped == 1 || (Finished == producers_count && queue->count == 0)){
		pthread_mutex_unlock(&mutex);
		return 0;
	}
	struct Message* res = queue->tail;
	if (queue->head == queue->tail) {
		queue->head = NULL;
		queue->tail = NULL;
	}
	else{
		queue->tail = queue->tail->prev;
		queue->tail->next = NULL;
	}
	strncpy(buf, res->message, bufSize);
	if (queue->count == 10)
		pthread_cond_broadcast(&cond);
	(queue->count)--;
	pthread_mutex_unlock(&mutex);
	return 1;
}

int mymsgput(Queue* queue, char* msg) {
	if (queue == NULL)
		return 0;
	pthread_mutex_lock(&mutex);
	while (queue->count >= 10 && queue->isDroped != 1)
		pthread_cond_wait(&cond, &mutex);
	if (queue->isDroped == 1){
		pthread_mutex_unlock(&mutex);
		return 0;
	}
	Message* new_mes;
	new_mes = (Message*)malloc(sizeof(Message));
	new_mes->prev = NULL;
	new_mes->next = NULL;
	//printf("count = %d\n", queue->count);
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
	if (queue->count == 0)
		pthread_cond_broadcast(&cond);
	(queue->count)++;
	pthread_mutex_unlock(&mutex);
	return 1;
}

void* producer(void* q) {
	Queue* queue = (Queue*)q;
	for (int i = 0; i < 100; i++) {
		char buf[40];
		sprintf(buf, "Produce msg %d from %lu", i, pthread_self());
		if (!mymsgput(queue, buf))
            break;
	}
	pthread_mutex_lock(&mutex);
	Finished +=1;
	pthread_mutex_unlock(&mutex);
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
    //while (!Exit)
        //pause();
    //mymsqdrop(&queue);

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
