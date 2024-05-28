#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <wait.h>

int main(int argc, char* argv[]) {

    if (argc < 2) {
		perror("Invalid arguments.");
		return 0;
	}

    pid_t pidfork, pidwait;
    int status;

    if( (pidfork = fork()) == 0)
        execl("/bin/cat", "cat", argv[1], (char *) 0);
    else
        printf("parent is waiting...\n");

    pidwait = wait(&status);

    printf("Child finished: WIFEXITED(status) is %d.\n", WIFEXITED(status));
    return 0;
}
