#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/////////////////////////////////////////////////////////////////////////////////
typedef struct list {
    char *str;
    struct list *next;
} list;

list* head = NULL;
list* tail = NULL;

void push (char *str) {
    list *newlst;
    newlst = (list*)malloc(sizeof(list));
    newlst->str = (char*) malloc((strlen(str) + 1) * sizeof(char));
    strcpy(newlst->str, str);
    newlst->next = NULL;
    if (head == NULL)
        head = newlst;
    if (tail != NULL)
        tail->next = newlst;
    tail = newlst;
}

void print() {
    while (head != NULL) {
        printf("%s", head->str);
        head = head->next;
    }
}

void pop() {
    while (head != NULL) {
        list *next;
        next = head->next;
        free(head->str);
        free(head);
        head = next->next;
    }
}

/////////////////////////////////////////////////////////////////////////////////
pthread_mutex_t mutex;

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

    pthread_mutex_lock(&mutex);
        push((char*)param);
    pthread_mutex_unlock(&mutex);

	pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
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
	while ((fgets(strings[str_count], 256, file) != NULL) && str_count < 100)
		str_count++;

    pthread_t* threads = malloc (sizeof(pthread_t) * str_count);
    int code;
    pthread_mutex_init(&mutex, NULL);

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

    print();
    pop();
    free(threads);
    pthread_mutex_destroy(&mutex);
    pthread_exit(NULL);
}

