#include "producer_consumer.h"
#include "readers_writers.h"
#include "philosophers.h"
#include "common.h"
#include "utils.h"
#include <string.h>
#include <stdio.h>

static void run_tests(void) {
    printf("\n========== PRODUCER-CONSUMER TEST ==========\n");
    run_producer_consumer(2, 2, 0);
    printf("\n========== READERS-WRITERS TEST ==========\n");
    run_readers_writers(3, 2, 0);
    printf("\n========== PHILOSOPHERS TEST ==========\n");
    run_philosophers(5, 0);
}

int main(int argc, char *argv[]) {
    if (argc > 1 && strcmp(argv[1], "test") == 0) {
        run_tests();
        return 0;
    }

    int choice;
    print_banner("Process Synchronization Simulator");
    printf("1. Producer-Consumer\n");
    printf("2. Readers-Writers (Reader First)\n");
    printf("3. Dining Philosophers\n");
    printf("4. Run All Tests\n");
    printf("Choice: ");
    if (scanf("%d", &choice) != 1) { WARN("Invalid input"); return 1; }

    switch (choice) {
        case 1: run_producer_consumer(2, 2, 0); break;
        case 2: run_readers_writers(3, 2, 0); break;
        case 3: run_philosophers(5, 0); break;
        case 4: run_tests(); break;
        default: WARN("Invalid choice."); return 1;
    }
    return 0;
}
