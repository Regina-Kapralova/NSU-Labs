#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define PHILO 5
#define DELAY 30000
#define FOOD 50

pthread_mutex_t forks[PHILO];
pthread_t phils[PHILO];
pthread_mutex_t foodlock;
int sleep_seconds;

int food_on_table () {
  static int food = FOOD;
  int myfood;

  pthread_mutex_lock (&foodlock);
  if (food > 0)
    food--;
  myfood = food;
  pthread_mutex_unlock (&foodlock);
  return myfood;
}

void get_fork (int phil, int fork, char *hand) {
  pthread_mutex_lock (&forks[fork]);
  printf ("Philosopher %d: got %s fork %d\n", phil, hand, fork);
}

void down_forks (int f1, int f2) {
  pthread_mutex_unlock (&forks[f1]);
  pthread_mutex_unlock (&forks[f2]);
}

void* philosopher (void* num) {
  int id = *(int*)num;

  int left_fork = (id + 1) % 5, right_fork = id, f;
  printf ("Philosopher %d sitting down to dinner.\n", id);

  while (f = food_on_table ()) {
    printf ("Philosopher %d: get dish %d.\n", id, f);
    if (id == 0) {
        get_fork (id, right_fork, "right");
        get_fork (id, left_fork, "left ");
    }
    else {
        get_fork (id, left_fork, "left ");
        get_fork (id, right_fork, "right");
    }

    printf ("Philosopher %d: eating.\n", id);
    usleep (DELAY * (FOOD - f + 1));
    down_forks (left_fork, right_fork);
  }
  printf ("Philosopher %d is done eating.\n", id);
  return (NULL);
}

int main (int argn, char **argv) {
  if (argn == 2)
    sleep_seconds = atoi (argv[1]);

  pthread_mutex_init (&foodlock, NULL);
  for (int i = 0; i < PHILO; i++)
    pthread_mutex_init (&forks[i], NULL);

  for (int i = 0; i < PHILO; i++)
    pthread_create(&phils[i], NULL, philosopher, &i);

  for (int i = 0; i < PHILO; i++)
    pthread_join (phils[i], NULL);
  return 0;
}








