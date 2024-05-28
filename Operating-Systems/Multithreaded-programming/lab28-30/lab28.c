#include "stdio.h"
#include "string.h"
#include "sys/socket.h"
#include <netdb.h>
#include <unistd.h>
#include "stdlib.h"
#include "errno.h"

#define MAX_STRING 25
#define SIZE_BUF 4096

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
    else {
        if (strstr(url, "https://"))
            start = 8;
        else {
            free(info);
            return NULL;
        }
    }
    end = start;
    while ((end < strlen(url)) && (url[end] != '/')) {
        if (url[end] == ':') {
            port_exist = 1;
            break;
        }
        end++;
    }
    info->host_name = (char*)malloc(sizeof(char) * (end - start) + 1);
    strncpy(info->host_name, url + start, end - start);
    if (port_exist) {
        int port = 0;
        end += 1;
        while ((url[end] >= '0') && (url[end] <= '9') && (end < strlen(url))) {
            port = port * 10 + url[end] - '0';
            end++;
        }
        info->port = port;
    }
    start = end;
    if (start != strlen(url)) {
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

char* make_headers(url_info* info) {
    char* head;
    int size = strlen(info->host_name) + strlen("GET  HTTP/1.0\r\nHost: ") + 5;
    if (info->path != NULL)
        size += strlen(info->path);
    else
        size += 1;
    head = (char*)malloc(sizeof(char) * size);
    memset(head, 0, size); // заполнение head нулями
    strcat(head, "GET "); // добавление "GET " в конец head
    if (info->path != NULL)
        strcat(head, info->path);
    else
        strcat(head, "/");
    strcat(head, " HTTP/1.0\r\nHost: ");
    strcat(strcat(head, info->host_name), "\r\n\r\n");
    printf("%s\n", head);
    return head;
}

int open_connection(url_info* info) {
    int socket_fd;
    struct addrinfo *hints, *res, *rp;
    char str_port[11];
    int result;
    hints = calloc(1, sizeof(struct addrinfo)); //указывает на предпочтительный тип сокета или протокол
    hints->ai_family = AF_UNSPEC; //произвольное семейство протоколов (например, IPv4 либо IPv6).
    hints->ai_socktype = SOCK_STREAM;
    snprintf(str_port, 11, "%d", info->port); //strcopy port в str_port

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

void urlViewer(char* url) {
    int socket_fd;
    char* headers;
    int result;
    int count_string = 0;
    int end = 0;
    int count_symbol_in_buf = 0;
    int count_written_symbol = 0;
    char buf[SIZE_BUF];

    url_info* info = url_parser(url);
    if (info == NULL) {
        perror("Bad url");
        return;
    }
    socket_fd = open_connection(info);
    headers = make_headers(info);
    send(socket_fd, headers, strlen(headers), 0);

    while (1) {
        fd_set reads, writes;
        FD_ZERO(&reads); //обнуляем структуры
        FD_ZERO(&writes);
        if (count_string >= MAX_STRING)
            //Как только у нас уже выведено 25 строк мы можем реагировать на нажатие клавиши
            FD_SET(0, &reads); //0 - это клавиатура
        if (count_symbol_in_buf != SIZE_BUF && !end)
            //Если у нас есть свободное место в буфере, то будем считывать данные с сокета
            FD_SET(socket_fd, &reads);

        result = select(socket_fd + 1, &reads, &writes, NULL, NULL);
        if (result < 0) {
            perror("select");
            close(socket_fd);
            return;
        }
        if (result > 0) { //У нас произошло какое-то собитие
            if (count_symbol_in_buf != SIZE_BUF && FD_ISSET(socket_fd, &reads)) {
                //У есть место в буфере и произошло событие на сокете
                result = recv(socket_fd, buf + count_symbol_in_buf, SIZE_BUF - count_symbol_in_buf, 0);
                if (result == 0)
                    end = 1;
                if (result == -1) {
                    perror("recv");
                    close(socket_fd);
                    return;
                }
                count_symbol_in_buf += result;
            }
            if (FD_ISSET(0, &reads) && count_string >= MAX_STRING) {
                char tmp[64];
                read(0, tmp, 64);
                count_string = 0;
            }
        }

        if (count_string < MAX_STRING && count_symbol_in_buf != count_written_symbol) {
            int tmp = 0;
            while(count_string < MAX_STRING && (count_written_symbol + tmp) < count_symbol_in_buf){
                if (buf[count_written_symbol + tmp] == '\n')
                    count_string++;
                tmp++;
            }
            write(1, buf + count_written_symbol, tmp);
            if (count_string == MAX_STRING)
                write(1, "Press Enter to continue", 23);
            count_written_symbol += tmp;
        }

        if (count_symbol_in_buf == count_written_symbol) {
            count_symbol_in_buf = 0;
            count_written_symbol = 0;
            if (end)
                break;
        }
    }
    close(socket_fd);
}

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Please, input URL\n");
        return 0;
    }
    urlViewer(argv[1]);
    return 0;
}

//fprintf() and errno поменять на printf()
