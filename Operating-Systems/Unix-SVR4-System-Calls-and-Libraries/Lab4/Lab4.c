#include <stdio.h>
#include <string.h>
#include <malloc.h>

typedef struct list {
    char *str;
    struct list *next;
} list;

list* push (list *lst, char *str) {
    list *newlst;
    newlst = (list*)malloc(sizeof(list));
    newlst->str = (char*) malloc((strlen(str) + 1) * sizeof(char));
    strcpy(newlst->str, str);
    newlst->next = NULL;
    if (lst != NULL) {
        list *tmp = lst->next;
        lst->next = newlst;
        newlst->next = tmp;
    }
    return newlst;
}

list* pop(list *head) {
    if (head == NULL)
        return NULL;
    list *next;
    next = head->next;
    free(head->str);
    free(head);
    return next;
}

int main() {
    char str[100];
    list *head = NULL, *tail = NULL;

    while (fgets(str, sizeof(str), stdin)) {
        if (str[0] == '.')
            break;
        tail = push(tail, str);
        if (head == NULL)
            head = tail;
    }

    while(head != NULL) {
        fputs(head->str, stdout);
        head = pop(head);
    }

    return 0;
}

