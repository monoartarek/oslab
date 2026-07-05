/*
 * Experiment 7: Readers-Writers Problem (Reader-priority solution)
 * Compile : gcc exp7_readers_writers.c -o rw -lpthread
 * Run     : ./rw
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

int NUM_READERS, NUM_WRITERS;
int shared_data = 0;
int read_count = 0;

sem_t rw_mutex;          /* controls access to shared_data for writers */
pthread_mutex_t mutex;   /* protects read_count */

void *reader(void *arg) {
    long id = (long)arg;

    pthread_mutex_lock(&mutex);
    read_count++;
    if (read_count == 1)
        sem_wait(&rw_mutex);   /* first reader locks out writers */
    pthread_mutex_unlock(&mutex);

    printf("Reader-%ld reads shared_data = %d\n", id, shared_data);
    sleep(1);

    pthread_mutex_lock(&mutex);
    read_count--;
    if (read_count == 0)
        sem_post(&rw_mutex);   /* last reader lets writers in */
    pthread_mutex_unlock(&mutex);

    return NULL;
}

void *writer(void *arg) {
    long id = (long)arg;

    sem_wait(&rw_mutex);
    shared_data += 10;
    printf("Writer-%ld writes shared_data = %d\n", id, shared_data);
    sleep(1);
    sem_post(&rw_mutex);

    return NULL;
}

int main() {
    printf("Enter number of readers: ");
    scanf("%d", &NUM_READERS);
    printf("Enter number of writers: ");
    scanf("%d", &NUM_WRITERS);

    sem_init(&rw_mutex, 0, 1);
    pthread_mutex_init(&mutex, NULL);

    pthread_t readers[NUM_READERS], writers[NUM_WRITERS];

    for (long i = 0; i < NUM_WRITERS; i++)
        pthread_create(&writers[i], NULL, writer, (void *)i);
    for (long i = 0; i < NUM_READERS; i++)
        pthread_create(&readers[i], NULL, reader, (void *)i);

    for (int i = 0; i < NUM_WRITERS; i++) pthread_join(writers[i], NULL);
    for (int i = 0; i < NUM_READERS; i++) pthread_join(readers[i], NULL);

    sem_destroy(&rw_mutex);
    pthread_mutex_destroy(&mutex);

    printf("Final shared_data = %d\n", shared_data);
    return 0;
}
