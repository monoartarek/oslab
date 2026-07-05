/*
 * Experiment 9: The Sleeping Barber Problem
 * Compile : gcc exp9_sleeping_barber.c -o barber -lpthread
 * Run     : ./barber
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

int CHAIRS;                 /* waiting-room chairs */
int NUM_CUSTOMERS;
int waiting = 0;

sem_t customers;    /* counts waiting customers, wakes barber   */
sem_t barber_ready;  /* counts free barber, wakes a customer     */
pthread_mutex_t mutex; /* protects "waiting" counter              */

void *barber(void *arg) {
    int served = 0;
    while (served < NUM_CUSTOMERS) {
        sem_wait(&customers);        /* sleep until a customer arrives */

        pthread_mutex_lock(&mutex);
        waiting--;
        pthread_mutex_unlock(&mutex);

        sem_post(&barber_ready);     /* invite the customer to the chair */

        printf("Barber is cutting hair...\n");
        usleep(500000);
        printf("Barber finished a haircut.\n");
        served++;
    }
    printf("Barber has served all customers and goes home.\n");
    return NULL;
}

void *customer(void *arg) {
    long id = (long)arg;

    pthread_mutex_lock(&mutex);
    if (waiting < CHAIRS) {
        waiting++;
        printf("Customer %ld sits in waiting room (waiting = %d)\n", id, waiting);
        pthread_mutex_unlock(&mutex);

        sem_post(&customers);        /* wake the barber            */
        sem_wait(&barber_ready);      /* wait for barber's turn      */

        printf("Customer %ld is getting a haircut.\n", id);
    } else {
        pthread_mutex_unlock(&mutex);
        printf("Customer %ld leaves - no chairs available.\n", id);
    }
    return NULL;
}

int main() {
    printf("Enter number of waiting-room chairs: ");
    scanf("%d", &CHAIRS);
    printf("Enter number of customers arriving: ");
    scanf("%d", &NUM_CUSTOMERS);

    sem_init(&customers, 0, 0);
    sem_init(&barber_ready, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    pthread_t barber_thread;
    pthread_create(&barber_thread, NULL, barber, NULL);

    pthread_t *cust = malloc(NUM_CUSTOMERS * sizeof(pthread_t));
    for (long i = 0; i < NUM_CUSTOMERS; i++) {
        pthread_create(&cust[i], NULL, customer, (void *)i);
        usleep(300000); /* stagger arrivals */
    }

    for (int i = 0; i < NUM_CUSTOMERS; i++)
        pthread_join(cust[i], NULL);
    pthread_join(barber_thread, NULL);

    sem_destroy(&customers);
    sem_destroy(&barber_ready);
    pthread_mutex_destroy(&mutex);
    free(cust);

    return 0;
}
