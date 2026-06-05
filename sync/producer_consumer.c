#include "producer_consumer.h"
#include "common.h"
#include "utils.h"
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 10

static int buffer[BUFFER_SIZE];
static int in = 0, out = 0;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static sem_t empty_slots;
static sem_t filled_slots;

static void *producer_thread(void *arg) {
    long id = (long)arg;
    for (int i = 0; i < 5; i++) {
        int item = id * 100 + i;
        sem_wait(&empty_slots);
        pthread_mutex_lock(&mutex);
        buffer[in] = item;
        printf("[Producer %ld] produced %d at slot %d\n", id, item, in);
        in = (in + 1) % BUFFER_SIZE;
        pthread_mutex_unlock(&mutex);
        sem_post(&filled_slots);
        usleep(100000);
    }
    return NULL;
}

static void *consumer_thread(void *arg) {
    long id = (long)arg;
    for (int i = 0; i < 5; i++) {
        sem_wait(&filled_slots);
        pthread_mutex_lock(&mutex);
        int item = buffer[out];
        printf("[Consumer %ld] consumed %d from slot %d\n", id, item, out);
        out = (out + 1) % BUFFER_SIZE;
        pthread_mutex_unlock(&mutex);
        sem_post(&empty_slots);
        usleep(150000);
    }
    return NULL;
}

void run_producer_consumer(int producers, int consumers, int items) {
    (void)items;
    sem_init(&empty_slots, 0, BUFFER_SIZE);
    sem_init(&filled_slots, 0, 0);
    pthread_t pts[8], cts[8];

    print_banner("Producer-Consumer Problem");
    printf("Producers: %d, Consumers: %d, Buffer: %d\n\n", producers, consumers, BUFFER_SIZE);

    for (long i = 0; i < producers; i++) {
        pthread_create(&pts[i], NULL, producer_thread, (void *)i);
    }
    for (long i = 0; i < consumers; i++) {
        pthread_create(&cts[i], NULL, consumer_thread, (void *)i);
    }
    for (int i = 0; i < producers; i++) pthread_join(pts[i], NULL);
    for (int i = 0; i < consumers; i++) pthread_join(cts[i], NULL);

    sem_destroy(&empty_slots);
    sem_destroy(&filled_slots);
    pthread_mutex_destroy(&mutex);
    printf("\nProducer-Consumer completed.\n");
}
