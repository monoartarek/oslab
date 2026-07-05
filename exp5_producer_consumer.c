/*
 * Experiment 5: Producer-Consumer Problem (using POSIX semaphores + threads)
 * Compile : gcc exp5_producer_consumer.c -o pc -lpthread
 * Run     : ./pc
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

int BUFFER_SIZE;
int *buffer;
int in = 0, out = 0;
int items_to_produce;

sem_t empty, full;
pthread_mutex_t mutex;

void *producer(void *arg) {
    for (int i = 1; i <= items_to_produce; i++) {
        sem_wait(&empty);                  /* wait for an empty slot   */
        pthread_mutex_lock(&mutex);

        buffer[in] = i;
        printf("Producer produced item %d at buffer index %d\n", i, in);
        in = (in + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex);
        sem_post(&full);                   /* signal a full slot       */
        sleep(1);
    }
    return NULL;
}

void *consumer(void *arg) {
    for (int i = 1; i <= items_to_produce; i++) {
        sem_wait(&full);                   /* wait for a full slot     */
        pthread_mutex_lock(&mutex);

        int item = buffer[out];
        printf("Consumer consumed item %d from buffer index %d\n", item, out);
        out = (out + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex);
        sem_post(&empty);                  /* signal an empty slot     */
        sleep(1);
    }
    return NULL;
}

int main() {
    printf("Enter buffer size: ");
    scanf("%d", &BUFFER_SIZE);
    printf("Enter number of items to produce/consume: ");
    scanf("%d", &items_to_produce);

    buffer = (int *)malloc(BUFFER_SIZE * sizeof(int));

    sem_init(&empty, 0, BUFFER_SIZE); /* all slots empty initially */
    sem_init(&full, 0, 0);            /* no slots full initially   */
    pthread_mutex_init(&mutex, NULL);

    pthread_t prod_thread, cons_thread;
    pthread_create(&prod_thread, NULL, producer, NULL);
    pthread_create(&cons_thread, NULL, consumer, NULL);

    pthread_join(prod_thread, NULL);
    pthread_join(cons_thread, NULL);

    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);
    free(buffer);

    printf("Production and consumption complete.\n");
    return 0;
}
