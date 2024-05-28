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

    if ( (pidfork = fork()) == 0) {
        execvp(argv[1], &argv[1]);
        perror(argv[1]);
        exit(1);
    }

    pidwait = wait(&status);

    printf("Child finished: WEXITSTATUS(status) is %d.\n", WEXITSTATUS(status));
    return 0;
}
