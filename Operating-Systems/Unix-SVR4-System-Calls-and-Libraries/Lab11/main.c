#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int execvpe(char *file, char *argv[], char *envp[]) {
    extern char** environ;
    environ = envp;
    execvp(file, argv);

    return -3;
}

int main(int argc, char* argv[]) {
    char * nenv[] = {"NAME=value", "nextname=nextvalue", "HOME=/xyz", (char*)0};
    char * argv1[] = {"testprg", "Success!"};
    execvpe(argv1[0], argv1, nenv);
             perror(argv[0]);
    return 0;
}
