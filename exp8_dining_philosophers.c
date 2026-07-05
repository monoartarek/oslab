/*
 * Experiment 8: Dining Philosophers Problem (deadlock-avoided solution)
 * Compile : gcc exp8_dining_philosophers.c -o dp -lpthread
 * Run     : ./dp
 *
 * Deadlock avoidance trick: the last philosopher picks up the
 * right fork first instead of the left fork.
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int N;                       /* number of philosophers */
pthread_mutex_t *forks;
int MEALS;                   /* number of times each philosopher eats */

void *philosopher(void *arg) {
    long id = (long)arg;
    int left = id;
    int right = (id + 1) % N;

    for (int meal = 1; meal <= MEALS; meal++) {
        printf("Philosopher %ld is thinking...\n", id);
        usleep(200000);

        if (id == N - 1) {
            /* last philosopher picks right fork first to avoid circular wait */
            pthread_mutex_lock(&forks[right]);
            pthread_mutex_lock(&forks[left]);
        } else {
            pthread_mutex_lock(&forks[left]);
            pthread_mutex_lock(&forks[right]);
        }

        printf("Philosopher %ld is EATING (meal %d)\n", id, meal);
        usleep(300000);

        pthread_mutex_unlock(&forks[left]);
        pthread_mutex_unlock(&forks[right]);
    }
    printf("Philosopher %ld has finished all meals.\n", id);
    return NULL;
}

int main() {
    printf("Enter number of philosophers: ");
    scanf("%d", &N);
    printf("Enter number of meals per philosopher: ");
    scanf("%d", &MEALS);

    forks = malloc(N * sizeof(pthread_mutex_t));
    pthread_t *phil = malloc(N * sizeof(pthread_t));

    for (int i = 0; i < N; i++)
        pthread_mutex_init(&forks[i], NULL);

    for (long i = 0; i < N; i++)
        pthread_create(&phil[i], NULL, philosopher, (void *)i);

    for (int i = 0; i < N; i++)
        pthread_join(phil[i], NULL);

    for (int i = 0; i < N; i++)
        pthread_mutex_destroy(&forks[i]);

    free(forks);
    free(phil);
    printf("All philosophers are done eating.\n");
    return 0;
}
