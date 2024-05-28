#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <libgen.h>
#include <sys/types.h>

int main(int argc, char** argv) {

    FILE* fptr[2];
    char num[4];

    srand(time(0));

    p2open("/bin/sort", fptr);

    int i;
    int count = 100;
    for(i = 0; i < 100; ++i) {
        sprintf(num, "%02d\n", rand() % 100);
        fputs(num, fptr[0]);
    }
    fclose(fptr[0]);

    i = 1;
    while(fgets(num, 4, fptr[1]) != NULL) {
        num[2] = '\0';
        printf(" %s", num);
        if(i++ == 10) {
            i = 1;
            printf("\n");
        }
    }
    fclose(fptr[1]);
    return 0;
}
