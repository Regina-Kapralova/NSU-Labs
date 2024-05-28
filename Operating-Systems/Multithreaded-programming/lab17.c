#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

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
} list;

list* head = NULL;

void push (char *str) {
    list *newlst;
    newlst = (list*)malloc(sizeof(list));
    newlst->string = (char*) malloc((strlen(str) + 1) * sizeof(char));
    strcpy(newlst->string, str);
    newlst->next = head;
    head = newlst;
}

void print() {
    if (head == NULL)
        return;
    printf("----------------------\n");
    for (list* iter = head; iter; iter = iter->next)
        printf("%s", iter->string);
    printf("----------------------\n");

}

void pop_all() {
    while (head != NULL) {
        list *next;
        next = head->next;
        free(head->string);
        free(head);
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
        pthread_mutex_lock(&mutex);
        int i = 0, j = 0;
        int cnt = 0;
        for (list* iter = head; iter; iter = iter->next, i++, j = 0, cnt = 0) {
            for (list* inner = head; inner->next; inner = inner->next, j++) {
                if (compare(inner->string, inner->next->string) > 0){
                    swap(&(inner->string), &(inner->next->string));
                    cnt++;
                }
            }
            if (cnt == 0)
                break;
        }
        pthread_mutex_unlock(&mutex);
    }
}

void read_push(){
    char str[256];
    while (Exit != 1){
        if (fgets(str, 256, stdin) == NULL)
            continue;
        pthread_mutex_lock(&mutex);
        if (strcmp("\n", str) == 0)
            print();
        else
            push(str);
        pthread_mutex_unlock(&mutex);
    }
}

int main() {
    signal(SIGINT, finish);

    pthread_t child;
    if (check_error(pthread_create(&child, NULL, sort, NULL)) == 1) {
        pop_all();
        exit(1);
    }
    read_push();

    if (check_error(pthread_join(child, NULL)) == 1) {
        pop_all();
        exit(1);
    }
    pop_all();
    pthread_exit(NULL);
}
