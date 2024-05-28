#include "stdio.h"
#include "string.h"
#include "sys/socket.h"
#include <netdb.h>
#include <unistd.h>
#include "stdlib.h"
#include "errno.h"
#include <aio.h>

#define MAX_STRING 25
#define SIZE_BUF 4096

typedef struct {
    char* host_name;
    int port;
    char* path;
} url_info;

url_info* url_parser(char* url) {
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

int open_connection(url_info* info) {
    int socket_fd;
    struct addrinfo* hints, * res, * rp;
    char str_port[11];
    int result;
    hints = calloc(1, sizeof(struct addrinfo));
    hints->ai_family = AF_UNSPEC;
    hints->ai_socktype = SOCK_STREAM;
    snprintf(str_port, 11, "%d", info->port);

    if ((result = getaddrinfo(info->host_name, str_port, hints, &res))) {
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
    int result, count;
    int count_string = 0;
    int end = 0;
    int count_symbol_in_buf = 0;
    int count_written_symbol = 0;
    char buf[SIZE_BUF];
    char t[64];
    char* message = "Press Enter to continue";
    struct aiocb write_rq, read_rq, pass, info_message;
    const struct aiocb* rq_mass[5];

    url_info* info = url_parser(url);
    if (info == NULL){
        perror("Bad url");
        return;
    }
    socket_fd = open_connection(info);
    headers = make_headers(info);
    send(socket_fd, headers, strlen(headers), 0);

    memset(&write_rq, 0, sizeof(struct aiocb)); //write_rq - вывод html (терминал)
    write_rq.aio_fildes = 1;
    memset(&read_rq, 0, sizeof(struct aiocb)); //read_rq - socket
    read_rq.aio_fildes = socket_fd;
    memset(&pass, 0, sizeof(struct aiocb)); //pass - нажатие клавиши
    pass.aio_fildes = 0;
    pass.aio_buf = t;
    memset(&info_message, 0, sizeof(struct aiocb)); //просьба нажать на клавишу (терминал)
    info_message.aio_fildes = 1;

    while (1){
        count = 0;
        memset(rq_mass, 0, sizeof(rq_mass));
        if (count_symbol_in_buf != SIZE_BUF && !end) {
            //Если есть свободное место в буфере, будем считывать данные с сокета
            read_rq.aio_buf = buf + count_symbol_in_buf;
            read_rq.aio_nbytes = SIZE_BUF - count_symbol_in_buf;
            aio_read(&read_rq);
            rq_mass[count] = &read_rq;
            count++;
        }
        else
            read_rq.aio_nbytes = 0;
        if (count_string == MAX_STRING) {
            //Как только выведено 25 строк мы можем реагировать на нажатие клавиши
            info_message.aio_buf = message;
            info_message.aio_nbytes = 23;
            aio_write(&info_message);
            pass.aio_nbytes = 1;
            aio_read(&pass);
            rq_mass[count] = &pass;
            count++;
        }
        else
            pass.aio_nbytes = 0;

        if (count_string < MAX_STRING && count_symbol_in_buf != count_written_symbol) {
            //Если мы ещё не вывели 25 строк и есть что выводить
            int tmp = 0;
            while(count_string < MAX_STRING && count_written_symbol + tmp < count_symbol_in_buf){
                if (buf[count_written_symbol + tmp] == '\n')
                    count_string++;
                tmp++;
            }
            write_rq.aio_buf = buf + count_written_symbol;
            write_rq.aio_nbytes = tmp;
            aio_write(&write_rq);
            rq_mass[count] = &write_rq;
            count++;
        }
        else
            write_rq.aio_nbytes = 0;

        aio_suspend(rq_mass, count, NULL);

        if ((aio_error(&info_message)) != EINPROGRESS) {} //чтобы выводилось press Enter
        // EINPROGRESS - запрос ещё не выполнен

        if (write_rq.aio_nbytes && (result = aio_error(&write_rq)) != EINPROGRESS) {
            //Если хотели что-то вывести и уже вывели
            if (result){ // != EINPROGRESS и != 0 => ошибка write
                errno = result;
                perror("Write");
                return;
            }
            result = aio_return(&write_rq);
            count_written_symbol += result;
        }

        if (read_rq.aio_nbytes && (result = aio_error(&read_rq)) != EINPROGRESS) {
            //Если хотели что-то получить и уже получили
            if (result){
                errno = result;
                perror("Read");
                return;
            }
            result = aio_return(&read_rq);
            if (result == 0)
                end = 1;
            else
                count_symbol_in_buf += result;
        }

        if (pass.aio_nbytes && (result = aio_error(&pass)) != EINPROGRESS) {
            //Если было нажатие на клавишу
            if (result){
                errno = result;
                perror("Read terminal");
                return;
            }
            count_string = 0;
        }
        if (count_symbol_in_buf == count_written_symbol){
            count_symbol_in_buf = 0;
            count_written_symbol = 0;
            if (end)
                break;
        }
    }
    close(socket_fd);
}

int main(int argc, char** argv){
    if (argc < 2){
        printf("Please, input URL\n");
        return 0;
    }
    urlViewer(argv[1]);
    return 0;
}

// Структура aio
//int aio_fildes– дескриптор файла
//off_t aio_offset– смещение в файле, начиная с которого будет идти запись или чтение
//volatile void* aio_buf– буфер, в который следует прочитать данные или в котором лежат данные для записи.
//size_t aio_nbytes– размер буфера.
//int aio_reqprio– приоритет запроса
//struct sigevent aio_sigevent– способ оповещения о завершении запроса
//int aio_lio_opcode– при aio_read(3AIO) и aio_write(3AIO) не используется, используется только функциейlio_listio.
