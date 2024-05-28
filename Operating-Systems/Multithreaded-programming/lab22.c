#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>

#define PHILO 5
#define DELAY 30000
#define FOOD 50

pthread_mutex_t forks[PHILO];
pthread_t phils[PHILO];
pthread_mutex_t foodlock;

pthread_mutex_t getting_forks;
pthread_cond_t cond;

void get_forks(int phil, int left_fork, int right_fork) {
    int fork_gotten;
    pthread_mutex_lock(&getting_forks);
    do {
        if ((fork_gotten = pthread_mutex_trylock(&forks[left_fork])) == 0){
            fork_gotten = pthread_mutex_trylock(&forks[right_fork]);
            if (fork_gotten != 0)
                pthread_mutex_unlock(&forks[left_fork]);
        }
        if (fork_gotten != 0)
            pthread_cond_wait(&cond, &getting_forks);
    } while (fork_gotten != 0);
    pthread_mutex_unlock(&getting_forks);
}

void down_forks(int left_fork, int right_fork) {
    pthread_mutex_lock(&getting_forks);
    pthread_mutex_unlock(&forks[left_fork]);
    pthread_mutex_unlock(&forks[right_fork]);
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&getting_forks);
}

int food_on_table() {
    static int food = FOOD;
    int myfood;

    pthread_mutex_lock(&foodlock);
    if (food > 0)
        food--;
    myfood = food;
    pthread_mutex_unlock(&foodlock);
    return myfood;
}

void* philosopher(void* num) {
    int id = *(int*)num;

    int left_fork = (id + 1) % 5, right_fork = id, f;
    printf ("Philosopher %d sitting down to dinner.\n", id);

    int cnt = 0;
    while (f = food_on_table()) {
        cnt++;
        printf("Philosopher %d: get dish %d.\n", id, f);
        get_forks(id, left_fork, right_fork);
        printf("Philosopher %d: eating.\n", id);
        usleep(DELAY * (FOOD - f + 1));
        down_forks(left_fork, right_fork);
    }
    printf("Philosopher %d is done eating\n", id);
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    pthread_mutex_init(&foodlock, NULL);
    pthread_mutex_init(&getting_forks, NULL);
    pthread_cond_init(&cond, NULL);

    for (int i = 0; i < PHILO; i++)
        pthread_mutex_init(&forks[i], NULL);

    for (int i = 0; i < PHILO; i++)
        pthread_create(&phils[i], NULL, philosopher, &i);

    for (int i = 0; i < PHILO; i++)
        pthread_join(phils[i], NULL);

    pthread_mutex_destroy(&getting_forks);
    pthread_mutex_destroy(&foodlock);
    pthread_cond_destroy(&cond);

    for (int i = 0; i < PHILO; i++)
        pthread_mutex_destroy(&forks[i]);

    pthread_exit(NULL);
}
