#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#define line_count 10
#define mutex_count 3
#define pid 0
#define cid 1

pthread_mutex_t mutex[mutex_count];
char* pline = "Hello from parent\n";
char* cline = "Hello from child\n";

int check_error (int code) {
    if (code != 0) {
        char error[256];
        strerror_r(code, error, sizeof(error));
        fprintf(stderr, "error: %d - %s\n", code, error);
        return 1;
    }
    else return 0;
}

int print_lines(char* line, int index) {
	int error;
	for (int i = 0; i < line_count; i++) {
		error = pthread_mutex_lock(&mutex[(2 * i + index) % mutex_count]);
		if (error != 0)
			return error;
		printf("%s", line);
		error = pthread_mutex_unlock(&mutex[(2 * i + 1 + index) % mutex_count]);
		if (error != 0)
			return error;
	}
	return 0;
}

void* thread_control(void* arg) {
	pthread_mutex_lock(&mutex[2]);
	check_error(print_lines(cline, cid));
	pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
	pthread_mutexattr_t mutex_attr;
	if (pthread_mutexattr_init(&mutex_attr) != 0) {
		printf("Pthread_mutexattr_init error\n");
		return 1;
	}
	if (pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_ERRORCHECK) != 0) {
		printf("Pthread_mutexattr_settype error\n");
		return 1;
	}
	for (int i = 0; i < mutex_count; i++)
		if (pthread_mutex_init(&mutex[i], &mutex_attr) != 0) {
			printf("Pthread_mutex_init error\n");
			return 1;
		}

	pthread_mutex_lock(&mutex[1]);

	pthread_t child_t;
	if (check_error(pthread_create(&child_t, NULL, thread_control, NULL)))
		return 1;

	if (check_error(print_lines(pline, pid)))
		return 1;

	for (int i = 0; i < mutex_count; i++) {
		if (pthread_mutex_destroy(&mutex[i]) != 0)
			return 1;
	}
	pthread_exit(NULL);
}

/*

--------------------------------------------------
	parent						child
--------------------------------------------------
	init(parent, 0, 1)
	init(child, 0, 0)
	start child
--------------------------------------------------
	wait(parent)			wait(child)
	print					print
	post(child)				post(parent)
--------------------------------------------------

----------------------------
	parent		child
----------------------------
	lock(0)
	start child
----------------------------
	lock(1)		lock(0)
	print		print
	unlock(0)	unlock(1)
-----------------------------
////////////////////////////////////////
----------------------------
	parent		child
----------------------------
	lock(1)		lock(2)
	start child
----------------------------
	lock(0)		lock(1)
	print		print
	unlock(1)	unlock(2)

	lock(2)		lock(0)
	print		print
	unlock(0)	unlock(1)

	lock(1)		lock(2)
	print		print
	unlock(2)	unlock(0)
----------------------------*/
