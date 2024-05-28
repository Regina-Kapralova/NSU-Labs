#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
extern char **environ;

int main(int argc, char* argv[]) {
    printf(argv[1]);
    printf("\nMy environment variables are:\n");
    char **p;
    for (p = environ; *p != 0; p++)
        printf("    %s\n", *p);
    return 0;
}
