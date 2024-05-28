#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

int main(int argc, char** argv) {
    pid_t pid;
    int fd[2];

    if (pipe(fd) == -1) {
        perror(argv[0]);
        return 0;
    }

    pid = fork();
    if (pid > 0) {
        close(fd[0]);
        char* message = { "It'S a woNdeRfUl daY!\n" };
        write(fd[1], message, strlen(message));
        close(fd[1]);
    }
    else if (pid == 0) {
        close(fd[1]);
        int n;
        char buffer[100];
        while ((n = read(fd[0], buffer, 100)) > 0) {
            for (int i = 0; i < n; i++)
                if (islower(buffer[i]))
                    buffer[i] = toupper(buffer[i]);
            write(0, buffer, n);
        }
        close(fd[0]);
    }
    else {
        perror(argv[0]);
        return 0;
    }
    return 0;
}
