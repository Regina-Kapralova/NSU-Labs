#include "stdio.h"
#include "string.h"
#include "sys/socket.h"
#include <netdb.h>
#include <unistd.h>
#include "stdlib.h"
#include "errno.h"
#include <pthread.h>

#define MAX_STRING 25
#define SIZE_BUF 4096

int socket_fd;
int end;
int count_symbol_in_buf;
int do_recv;
char buf[SIZE_BUF];
pthread_cond_t condVar;
pthread_mutex_t mutex;

typedef struct {
    char* host_name;
    int port;
    char* path;
} url_info;

url_info* url_parser(char* url){
    url_info* info = (url_info*)malloc(sizeof(url_info));
    int start, end, port_exist = 0;
    info->port = 80;
    info->path = NULL;
    if (strstr(url, "http://"))
        start = 7;
    else{
        if (strstr(url, "https://"))
            start = 8;
        else{
            free(info);
            return NULL;
        }
    }
    end = start;
    while ((end < strlen(url)) && (url[end] != '/')){
        if (url[end] == ':'){
            port_exist = 1;
            break;
        }
        end++;
    }
    info->host_name = (char*)malloc(sizeof(char) * (end - start) + 1);
    strncpy(info->host_name, url + start, end - start);
    if (port_exist){
        int port = 0;
        end += 1;
        while ((url[end] >= '0') && (url[end] <= '9') && (end < strlen(url))){
            port = port * 10 + url[end] - '0';
            end++;
        }
        info->port = port;
    }
    start = end;
    if (start != strlen(url)){
        info->path = (char*)malloc(sizeof(char) * (strlen(url) - start) + 1);
        strncpy(info->path, url + start, strlen(url) - start);
    }
    printf("Host: %s\n", info->host_name);
    printf("Port: %d\n", info->port);
    if (info->path != NULL)
        printf("Path: %s\n\n", info->path);
    else
        printf("Path: no path\n\n");
    return info;
}

char* make_headers(url_info* info){
    char* head;
    int size = strlen(info->host_name) + strlen("GET  HTTP/1.0\r\nHost: ") + 5;
    if (info->path != NULL)
        size += strlen(info->path);
    else
        size += 1;
    head = (char*)malloc(sizeof(char) * size);
    memset(head, 0, size);
    strcat(head, "GET ");
    if (info->path != NULL)
        strcat(head, info->path);
    else
        strcat(head, "/");
    strcat(head, " HTTP/1.0\r\nHost: ");
    strcat(strcat(head, info->host_name), "\r\n\r\n");
    printf("%s\n", head);
    return head;
}

int open_connection(url_info* info){
    int socket_fd;
    struct addrinfo* hints, * res, * rp;
    char str_port[11];
    int result;
    hints = calloc(1, sizeof(struct addrinfo));
    hints->ai_family = AF_UNSPEC;
    hints->ai_socktype = SOCK_STREAM;
    snprintf(str_port, 11, "%d", info->port);

    if ((result = getaddrinfo(info->host_name, str_port, hints, &res))){
        errno = result;
        perror("getaddrinfo");
        free(hints);
        exit(0);
    }
    for (rp = res; rp != NULL; rp = rp->ai_next){
        socket_fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (socket_fd == -1)
            continue;
        if (connect(socket_fd, rp->ai_addr, rp->ai_addrlen) == 0)
            break;
        close(socket_fd);
        free(hints);
        freeaddrinfo(res);
        free(rp);
        perror("socket");
        exit(0);
    }
    free(hints);
    freeaddrinfo(res);
    free(rp);
    return socket_fd;
}

void* receiver(void* args){
    while (1){
        pthread_mutex_lock(&mutex);
        if (do_recv == 0)
            pthread_cond_wait(&condVar, &mutex);
        if (do_recv == -1){
            close(socket_fd);
            pthread_exit(NULL);
        }
        do_recv = 1; //делаем recv, поэтому ставим 1 на время
        int result = recv(socket_fd, buf + count_symbol_in_buf, SIZE_BUF - count_symbol_in_buf, 0);
        if (result == 0)
            end = 1;
        if (result == -1){
            do_recv = -1; //типа ошибка
            perror("recv");
            close(socket_fd);
            pthread_exit(NULL);
        }
        count_symbol_in_buf += result;
        do_recv = 0;
        pthread_cond_signal(&condVar);
        pthread_mutex_unlock(&mutex);
    }
}

void urlViewer(char* url){
    char* headers;
    int result;
    int count_string = 0;
    end = 0; //global int
    count_symbol_in_buf = 0; //global int
    int count_written_symbol = 0;
    do_recv = 0;

    url_info* info = url_parser(url);
    if (info == NULL){
        perror("Bad url");
        return;
    }
    socket_fd = open_connection(info);
    headers = make_headers(info);
    send(socket_fd, headers, strlen(headers), 0);

    pthread_t thread;
    int error_code = pthread_create(&thread, NULL, receiver, NULL);
    if (error_code != 0)
    {
        //print_error(error_code, argv);
    }

    int socket_fd_ = socket_fd;
    while (1){
        fd_set reads, writes;
        FD_ZERO(&reads); //обнуляем структуры
        FD_ZERO(&writes);
        if (count_string >= MAX_STRING) //Как только у нас уже выведено больше 25 строк мы можем начать реагировать на нажатие клавиши
            FD_SET(0, &reads); //0 - это клавиатура
        if (count_symbol_in_buf != SIZE_BUF && !end) {
            //Если у нас есть свободное место в буфере, то будем считывать данные с сокета
            FD_SET(socket_fd_, &reads);
            pthread_cond_signal(&condVar); //делаем receiving()
        }
        result = select(socket_fd_ + 1, &reads, &writes, NULL, NULL);
        if (result < 0){
            perror("select");
            close(socket_fd);
            return;
        }
        if (result > 0) { //У нас произошло какое-то собитие
            if (FD_ISSET(0, &reads) && count_string >= MAX_STRING) {
                //Мы уже вывели 25 строчек и теперь считываем нажатие клавиатуры
                char tmp[64];
                read(0, tmp, 64);
                count_string = 0;
            }
        }
        pthread_mutex_lock(&mutex);
        if (do_recv == 1)
            pthread_cond_wait(&condVar, &mutex);
        if (count_string < MAX_STRING && count_symbol_in_buf != count_written_symbol) {
            //Если мы ещё не вывели (или не до конца вывели) 25 строк и у нас есть что вообще выводить
            int tmp = 0;
            while (count_string < MAX_STRING && count_written_symbol + tmp < count_symbol_in_buf){
                if (buf[count_written_symbol + tmp] == '\n')
                    count_string++;
                tmp++;
            }
            write(1, buf + count_written_symbol, tmp);
            if (count_string == MAX_STRING)
                write(1, "Press Enter to continue", 23);
            count_written_symbol += tmp;
        }

        if (count_symbol_in_buf == count_written_symbol) { //Всё что было в буфере - вывели
            count_symbol_in_buf = 0;
            count_written_symbol = 0;
            if (end){
                pthread_mutex_unlock(&mutex);
                break;
            }
        }
        pthread_mutex_unlock(&mutex);
    }
    do_recv = -1;
    pthread_cond_signal(&condVar);
}

int main(int argc, char** argv) {
    if (argc < 2){
        printf("Please, input URL\n");
        return 0;
    }
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&condVar, NULL);
    urlViewer(argv[1]);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&condVar);
    pthread_exit(NULL);
}
