#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/devpoll.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_CONNECT 510

struct pollfd polls[MAX_CONNECT * 2 + 2];
// 1024 - максимальное количество tcp соединений - максимальное количество открытых дискрипторов
int listening;
int clients_num = 0;
struct sockaddr* clients_addr = NULL;
struct sockaddr* clients_addr_lens = NULL;

void close_connection(int id){
    polls[id].fd = -1;
    polls[id - MAX_CONNECT].fd = -1;
    close(polls[id].fd);
    close(polls[id - MAX_CONNECT].fd);
    clients_num--;
}

int get_descriptor(struct pollfd* clients_polls){
    for (int i = 0; i < MAX_CONNECT; ++i){
        if (clients_polls[i].fd == -1)
            return i;
    }
    return -1;
}

void correctExit(int sig){
    if (sig == SIGINT){
        shutdown(listening, SHUT_RDWR);
        close(listening);
        free(clients_addr);
        free(clients_addr_lens);
        exit(0);
    }
}

int main(int argc, char* argv[]){
    signal(SIGINT, correctExit);
    if (argc != 5){
        printf("Input: <listening ip-address> <listening port> <ip-address translation> <port translation>\n");
        exit(-1);
    }
    char* ip_s = argv[1]; //l(ifconfig) 10.4.0.68
    char* port_s = argv[2];
    char* server_ip_s = argv[3];; // (dig parallels.nsu.ru) 84.237.52.62; 74.125.131.102   173.194.73.139 - google
    char* server_port_s = argv[4]; //80
    if (ip_s == NULL){
        printf("Error in <listening ip-address>\n");
        return 0;
    }
    if (port_s == NULL){
        printf("Error in <listening port>\n");
        return 0;
    }
    if (server_ip_s == NULL){
        printf("Error in <ip-address translation>\n");
        return 0;
    }
    if (server_port_s == NULL){
        printf("Error in <port translation>\n");
        return 0;
    }
    unsigned short listening_port = strtoul(port_s, NULL, 10);
    if ((listening_port < 49152) || (listening_port > 65535)){
        printf("Port must be in [ 49152; 65535 ]\n");
        return 0;
    }
    unsigned short server_port = strtoul(server_port_s, NULL, 10);

    if ((listening = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("Error in create socket for listening");
        return 0;
    }
    struct sockaddr_in listening_addr;
    listening_addr.sin_family = AF_INET;
    listening_addr.sin_addr.s_addr = inet_addr(ip_s);
    listening_addr.sin_port = htons(listening_port);
    if (bind(listening, (struct sockaddr*)&listening_addr, sizeof(listening_addr)) == -1){
        perror("Error in bind for listening");
        return 0;
    }
    if (listen(listening, MAX_CONNECT) == -1) {
        perror("Error in listen for listening");
        return 0;
    }
    printf("Translation server started successfully!\n");

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(server_ip_s);
    server_addr.sin_port = htons(server_port);

    for (int i = 0; i < MAX_CONNECT * 2 + 2; ++i)
        polls[i].events = POLLIN;
    for (int i = 0; i < MAX_CONNECT * 2; ++i)
        polls[i].fd = -1;
    polls[MAX_CONNECT * 2 + 1].fd = listening;

    clients_addr = (struct sockaddr*)malloc(MAX_CONNECT * sizeof(struct sockaddr));
    clients_addr_lens = (struct sockaddr*)malloc(MAX_CONNECT * sizeof(struct sockaddr));
    while (1){
        int respondClients = poll((struct pollfd*)&polls, MAX_CONNECT * 2 + 2, -1);
        if ((respondClients == -1) || (respondClients == 0)){
            close(listening);
            perror("Error in poll");
            exit(1);
        }
        if (polls[MAX_CONNECT * 2 + 1].revents == POLLIN){
            polls[MAX_CONNECT * 2 + 1].revents = 0;
            if (clients_num < MAX_CONNECT){
                int id;
                if ((id = get_descriptor(polls)) == -1)
                    printf("No free descriptors\n");
                polls[id].fd = accept(listening, &clients_addr[clients_num], (socklen_t*)&clients_addr_lens[clients_num]);
                //printf("Client here!\n");
                polls[id + MAX_CONNECT].fd = socket(AF_INET, SOCK_STREAM, 0);
                if (connect(polls[id + MAX_CONNECT].fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
                    perror("Error in cinnect");
                    exit(-1);
                }
                clients_num++;
            }
        }
        for (int i = 0; i < clients_num; ++i){
            if ((polls[i].fd == -1) || (polls[i].revents != POLLIN))
                continue;
            polls[i].revents = 0;
            int num_read;
            char buffer[BUFSIZ];
            if ((num_read = read(polls[i].fd, buffer, BUFSIZ)) > 0){
                if (write(polls[i + MAX_CONNECT].fd, buffer, num_read) != num_read)
                    close_connection(i);
            }
            else
                close_connection(i);
        }
        for (int i = MAX_CONNECT; i < MAX_CONNECT * 2; ++i){
            if ((polls[i].fd == -1) || (polls[i].revents != POLLIN))
                continue;
            polls[i].revents = 0;
            int num_read;
            char buffer[BUFSIZ];
            if ((num_read = read(polls[i].fd, buffer, BUFSIZ)) > 0) {
                if (write(polls[i - MAX_CONNECT].fd, buffer, num_read) != num_read)
                    close_connection(i);
            }
            else
                close_connection(i);
        }
    }
}
//https://phys.bspu.by/static/um/inf/prg/sem3/book/vcpp/visual/16.html
