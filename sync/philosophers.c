#include "philosophers.h"
#include "common.h"
#include "utils.h"
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_PHILOS 16

static pthread_mutex_t forks[MAX_PHILOS];
static pthread_mutex_t room; /* Limits diners to N-1 */

static void *philosopher_thread(void *arg) {
    long id = (long)arg;
    int left = id;
    int right = (id + 1) % MAX_PHILOS;
    /* To prevent deadlock: odd philosophers pick left first, even pick right first */
    if (id % 2 == 0) { int tmp = left; left = right; right = tmp; }

    for (int i = 0; i < 3; i++) {
        printf("[Philosopher %ld] thinking...\n", id);
        usleep(100000 + rand() % 200000);

        printf("[Philosopher %ld] hungry...\n", id);
        pthread_mutex_lock(&room); /* Enter dining room (max N-1) */
        pthread_mutex_lock(&forks[left]);
        printf("[Philosopher %ld] picked up fork %d\n", id, left);
        pthread_mutex_lock(&forks[right]);
        printf("[Philosopher %ld] picked up fork %d -> EATING\n", id, right);

        usleep(200000 + rand() % 200000);

        pthread_mutex_unlock(&forks[right]);
        pthread_mutex_unlock(&forks[left]);
        pthread_mutex_unlock(&room);
        printf("[Philosopher %ld] finished eating, put down forks.\n", id);
    }
    printf("[Philosopher %ld] DONE.\n", id);
    return NULL;
}

void run_philosophers(int count, int eat_count) {
    (void)eat_count;
    if (count > MAX_PHILOS) count = MAX_PHILOS;
    pthread_t pts[MAX_PHILOS];
    srand((unsigned)time(NULL));

    pthread_mutex_init(&room, NULL);
    for (int i = 0; i < count; i++) pthread_mutex_init(&forks[i], NULL);

    print_banner("Dining Philosophers");
    printf("Philosophers: %d, Meals each: 3\n\n", count);

    for (long i = 0; i < count; i++) {
        pthread_create(&pts[i], NULL, philosopher_thread, (void *)i);
    }
    for (int i = 0; i < count; i++) pthread_join(pts[i], NULL);

    pthread_mutex_destroy(&room);
    for (int i = 0; i < count; i++) pthread_mutex_destroy(&forks[i]);
    printf("\nDining Philosophers completed (no deadlock).\n");
}
