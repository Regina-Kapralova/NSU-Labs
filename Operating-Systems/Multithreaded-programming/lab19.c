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
    pthread_mutex_t mutex;
} list;

list* head = NULL;

list* init() {
    head = (list*)malloc(sizeof(list));
    head->string = NULL;
    head->next = NULL;
    pthread_mutex_init(&(head->mutex), NULL);
    return head;
}

void push(char *str) {
    list *newlst;
    newlst = (list*)malloc(sizeof(list));
    newlst->string = (char*) malloc((strlen(str) + 1) * sizeof(char));
    strcpy(newlst->string, str);
    pthread_mutex_init(&(newlst->mutex), NULL);

    pthread_mutex_lock(&(head->mutex));
    newlst->next = head->next;
    head->next = newlst;
    pthread_mutex_unlock(&(head->mutex));
}

void print() {
    if (head == NULL)
        return;
    pthread_mutex_lock(&(head->mutex));
    list* prev = head;
    printf("----------------------\n");
    for (list* iter = head->next; iter; iter = iter->next) {
        pthread_mutex_lock(&(iter->mutex));
        pthread_mutex_unlock(&(prev->mutex));
        printf("%s", iter->string);
        prev = iter;
    }
    pthread_mutex_unlock(&(prev->mutex));
    printf("----------------------\n");
}

void pop_all() {
    while (head != NULL) {
        list *next;
        next = head->next;
        free(head->string);
        free(head);
        pthread_mutex_destroy(&(head->mutex));
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
        int i = 0, j = 0;
        int cnt = 0;
        for (list* iter = head->next; iter; iter = iter->next, i++, j = 0, cnt = 0) {
            list* prev = head;
            pthread_mutex_lock(&(head->mutex));
            pthread_mutex_lock(&(head->next->mutex));
            for (list* inner = head->next; inner->next; inner = inner->next, j++) {
                pthread_mutex_lock(&(inner->next->mutex));
                if (compare(inner->string, inner->next->string) > 0){
                    swap(&(inner->string), &(inner->next->string));
                    cnt++;
                    pthread_mutex_unlock(&(prev->mutex));
                    pthread_mutex_unlock(&(prev->next->mutex));
                    pthread_mutex_unlock(&(prev->next->next->mutex));
                    usleep(100000);
                    if (Exit)
                        pthread_exit(NULL);
                    pthread_mutex_lock(&(prev->mutex));
                    pthread_mutex_lock(&(prev->next->mutex));
                    pthread_mutex_lock(&(prev->next->next->mutex));
                }
                pthread_mutex_unlock(&(prev->mutex));
                prev = inner;
            }
            pthread_mutex_unlock(&(prev->mutex));
            pthread_mutex_unlock(&(prev->next->mutex));
            if (cnt == 0)
                break;
        }
    }
}

void read_push(){
    char str[256];
    head = init();
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
    signal(SIGINT, finish);

    int count;
    if (argc < 2)
        count = 1;
    else
        count = atoi(argv[1]);

    pthread_t* threads;
    threads = (pthread_t*)malloc(sizeof(pthread_t) * count);
    for (int i = 0; i < count; i++) {
        if (check_error(pthread_create(&threads[i], NULL, sort, NULL)) == 1) {
            pop_all();
            exit(1);
        }
    }
    read_push();

    for (int i = 0; i < count; i++) {
        if (check_error(pthread_join(threads[i], NULL)) == 1) {
            pop_all();
            exit(1);
        }
    }
    pop_all();
    pthread_exit(NULL);
}
