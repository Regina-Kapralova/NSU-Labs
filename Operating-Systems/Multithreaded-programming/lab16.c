#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>

sem_t* parent;
sem_t* child;

void parent_run() {
	for (int i = 0; i < 10; i++) {
		sem_wait(parent);
		printf("Hello from parent\n");
		sem_post(child);
	}
}

void child_run() {
	for (int i = 0; i < 10; i++) {
		sem_wait(child);
		printf("Hello from child\n");
		sem_post(parent);
	}
}

int main() {
	parent = sem_open("/parent", O_CREAT, O_RDWR, 1);
	child = sem_open("/child", O_CREAT, O_RDWR, 0);

	if (parent == SEM_FAILED || child == SEM_FAILED) {
		printf("Error in sem_open.\n");
		return 1;
	}

	if (fork() == 0) {
		child_run();
		if (sem_unlink("/child") != 0) {
			printf("Error in unlink child.\n");
			return 1;
		}
		sem_close(child);
	}
	else {
		parent_run();
		if (sem_unlink("/parent") != 0) {
			printf("Error in unlink parent.\n");
			return 1;
		}
		sem_close(parent);
	}
	return 0;
}
