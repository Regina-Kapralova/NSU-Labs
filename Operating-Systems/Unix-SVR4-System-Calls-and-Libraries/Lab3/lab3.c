#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char* argv[]) {

    printf("    User id: %d and effective user id: %d\n", getuid(), geteuid());

    FILE* file;
    file = fopen(argv[1], "r");
    if (file == NULL)
        perror("    File can't be opened on the first try");
    else {
        printf("    The file opens on the first try!\n\n");
        fclose(file);
    }

    seteuid(getuid());
    printf("    New user id: %d and new effective user id: %d\n", getuid(), geteuid());

    file = fopen (argv[1], "r");
    if (file == NULL)
        perror("    File can't be opened on the second try");
    else {
        printf("    The file opens on the second try!\n");
        fclose(file);
    }
    return 0;
 }

