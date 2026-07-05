/*
 * Experiment 6: Bounded-Buffer Problem (multiple producers & consumers)
 * Compile : gcc exp6_bounded_buffer.c -o bb -lpthread
 * Run     : ./bb
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

int BUFFER_SIZE, NUM_PRODUCERS, NUM_CONSUMERS, ITEMS_PER_PRODUCER;
int *buffer;
int in = 0, out = 0;
int produced_count = 0;

sem_t empty, full;
pthread_mutex_t mutex;

void *producer(void *arg) {
    long id = (long)arg;
    for (int i = 1; i <= ITEMS_PER_PRODUCER; i++) {
        int item = (int)(id * 1000 + i);

        sem_wait(&empty);
        pthread_mutex_lock(&mutex);

        buffer[in] = item;
        printf("Producer-%ld produced item %d at index %d\n", id, item, in);
        in = (in + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex);
        sem_post(&full);
        usleep(200000);
    }
    return NULL;
}

void *consumer(void *arg) {
    long id = (long)arg;
    int total_items = NUM_PRODUCERS * ITEMS_PER_PRODUCER;

    while (1) {
        pthread_mutex_lock(&mutex);
        if (produced_count >= total_items) {
            pthread_mutex_unlock(&mutex);
            break;
        }
        produced_count++;
        pthread_mutex_unlock(&mutex);

        sem_wait(&full);
        pthread_mutex_lock(&mutex);

        int item = buffer[out];
        printf("Consumer-%ld consumed item %d from index %d\n", id, item, out);
        out = (out + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
        usleep(300000);
    }
    return NULL;
}

int main() {
    printf("Enter buffer size: ");
    scanf("%d", &BUFFER_SIZE);
    printf("Enter number of producer threads: ");
    scanf("%d", &NUM_PRODUCERS);
    printf("Enter number of consumer threads: ");
    scanf("%d", &NUM_CONSUMERS);
    printf("Enter number of items each producer will produce: ");
    scanf("%d", &ITEMS_PER_PRODUCER);

    buffer = (int *)malloc(BUFFER_SIZE * sizeof(int));

    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    pthread_t prod[NUM_PRODUCERS], cons[NUM_CONSUMERS];

    for (long i = 0; i < NUM_PRODUCERS; i++)
        pthread_create(&prod[i], NULL, producer, (void *)i);
    for (long i = 0; i < NUM_CONSUMERS; i++)
        pthread_create(&cons[i], NULL, consumer, (void *)i);

    for (int i = 0; i < NUM_PRODUCERS; i++) pthread_join(prod[i], NULL);
    for (int i = 0; i < NUM_CONSUMERS; i++) pthread_join(cons[i], NULL);

    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);
    free(buffer);

    printf("All producers and consumers finished.\n");
    return 0;
}
