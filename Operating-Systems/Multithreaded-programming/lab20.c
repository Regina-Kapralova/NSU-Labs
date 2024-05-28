#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

int Exit = 0;
void finish(int sign) {
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

/////////////////////////////////////////////////////////////////////////////////
typedef struct list {
    char *string;
    struct list *next;
    pthread_rwlock_t rwlock;
} list;

list* head = NULL;

list* init() {
    head = (list*)malloc(sizeof(list));
    head->string = NULL;
    head->next = NULL;
    pthread_rwlock_init(&(head->rwlock), NULL);
    return head;
}

void push(char *str) {
    list *newlst;
    newlst = (list*)malloc(sizeof(list));
    newlst->string = (char*) malloc((strlen(str) + 1) * sizeof(char));
    strcpy(newlst->string, str);
    pthread_rwlock_init(&(head->rwlock), NULL);

    pthread_rwlock_wrlock(&(head->rwlock));
    newlst->next = head->next;
    head->next = newlst;
    pthread_rwlock_unlock(&(head->rwlock));
}

void print() {
    if (head == NULL)
        return;
    pthread_rwlock_rdlock(&(head->rwlock));
    list* prev = head;
    printf("----------------------\n");
    for (list* iter = head->next; iter; iter = iter->next) {
        pthread_rwlock_rdlock(&(iter->rwlock));
        pthread_rwlock_unlock(&(prev->rwlock));
        printf("%s", iter->string);
        prev = iter;
    }
    pthread_rwlock_unlock(&(prev->rwlock));
    printf("----------------------\n");
}

void pop_all() {
    while (head != NULL) {
        list *next;
        next = head->next;
        free(head->string);
        free(head);
        pthread_rwlock_destroy(&(head->rwlock));
        head = next;
    }
}
/////////////////////////////////////////////////////////////////////////////////

void swap(char** left, char** right){
    char* tmp = *left;
    *left = *right;
    *right = tmp;
}

int compare(char* left, char* right){
    int left_l = strlen(left), right_l = strlen(right);
    int minLength = (left_l > right_l) ? right_l : left_l;
    int maxLength = (left_l < right_l) ? right_l : left_l;
    for (int i = 0; i < minLength; i++) {
        if (left[i] != right[i])
            return left[i] - right[i];
    }
    if ((maxLength - minLength) == 0)
        return 0;
    return maxLength - strlen(left);
}

void* sort(void* data) {
    while (1) {
        if (Exit)
            pthread_exit(NULL);
        sleep(5);
        int cnt = 0;
        for (list* iter = head->next; iter; iter = iter->next, cnt = 0) {
            list* prev = head;
            pthread_rwlock_wrlock(&(head->rwlock));
            pthread_rwlock_wrlock(&(head->next->rwlock));
            for (list* inner = head->next; inner->next; inner = inner->next) {
                pthread_rwlock_wrlock(&(inner->next->rwlock));
                if (compare(inner->string, inner->next->string) > 0){
                    swap(&(inner->string), &(inner->next->string));
                    cnt++;
                }
                pthread_rwlock_unlock(&(prev->rwlock));
                prev = inner;
            }
            pthread_rwlock_unlock(&(prev->rwlock));
            pthread_rwlock_unlock(&(prev->next->rwlock));
            if (cnt == 0)
                break;
        }
    }
}

void read_push(FILE* file){
	char strings[256][100];
	int str_count = 0;
	while ((fgets(strings[str_count], 256, file) != NULL) && str_count < 100)
		str_count++;

    head = init();
    for (int i = 0; i < str_count; i++){
        push(strings[i]);
    }

    char str[256];
    while (Exit != 1){
        if (fgets(str, 256, stdin) == NULL)
            continue;
        if (strcmp("\n", str) == 0)
            print();
        else
            push(str);
    }
}

int main(int argc, char** argv) {
	if (argc < 2) {
        printf("Please, enter file's name.\n");
		return 0;
    }
    FILE* file;
    if ((file = fopen(argv[1], "r")) == NULL){
        printf("Please, enter correct file's name.\n", argv[1]);
        return 0;
    }

    signal(SIGINT, finish);

    int count = 1;
    /*if (argc < 2)
        count = 1;
    else
        count = atoi(argv[1]);*/

    pthread_t* threads;
    threads = (pthread_t*)malloc(sizeof(pthread_t) * count);
    for (int i = 0; i < count; i++) {
        if (check_error(pthread_create(&threads[i], NULL, sort, NULL)) == 1) {
            pop_all();
            exit(1);
        }
    }
    read_push(file);

    for (int i = 0; i < count; i++) {
        if (check_error(pthread_join(threads[i], NULL)) == 1) {
            pop_all();
            exit(1);
        }
    }
    fclose(file);
    pop_all();
    pthread_exit(NULL);
}
