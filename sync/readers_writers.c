#include "readers_writers.h"
#include "common.h"
#include "utils.h"
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t rw_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;
static int read_count = 0;

static void *reader_thread(void *arg) {
    long id = (long)arg;
    pthread_mutex_lock(&queue_mutex);
    pthread_mutex_unlock(&queue_mutex);

    pthread_mutex_lock(&mutex);
    read_count++;
    if (read_count == 1) pthread_mutex_lock(&rw_mutex);
    pthread_mutex_unlock(&mutex);

    printf("[Reader %ld] reading...\n", id);
    usleep(200000);
    printf("[Reader %ld] done.\n", id);

    pthread_mutex_lock(&mutex);
    read_count--;
    if (read_count == 0) pthread_mutex_unlock(&rw_mutex);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

static void *writer_thread(void *arg) {
    long id = (long)arg;
    pthread_mutex_lock(&queue_mutex);
    pthread_mutex_lock(&rw_mutex);
    pthread_mutex_unlock(&queue_mutex);

    printf("[Writer %ld] writing...\n", id);
    usleep(300000);
    printf("[Writer %ld] done.\n", id);

    pthread_mutex_unlock(&rw_mutex);
    return NULL;
}

void run_readers_writers(int readers, int writers, int writer_first) {
    pthread_t rts[8], wts[8];
    print_banner("Readers-Writers Problem");
    printf("Readers: %d, Writers: %d, %s\n\n",
           readers, writers, writer_first ? "Writer First" : "Reader First");

    for (long i = 0; i < readers; i++) {
        pthread_create(&rts[i], NULL, reader_thread, (void *)i);
    }
    for (long i = 0; i < writers; i++) {
        pthread_create(&wts[i], NULL, writer_thread, (void *)i);
    }
    for (int i = 0; i < readers; i++) pthread_join(rts[i], NULL);
    for (int i = 0; i < writers; i++) pthread_join(wts[i], NULL);

    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&rw_mutex);
    pthread_mutex_destroy(&queue_mutex);
    printf("\nReaders-Writers completed.\n");
}
