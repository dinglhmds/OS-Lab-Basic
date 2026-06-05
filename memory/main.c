#include "paging.h"
#include "partition.h"
#include "common.h"
#include "utils.h"
#include <string.h>

static void paging_demo(void) {
    print_banner("Page Replacement Demo");
    int ref_string[] = {1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5};
    int len = sizeof(ref_string) / sizeof(ref_string[0]);
    int frame_count = 3;

    printf("Reference String: ");
    for (int i = 0; i < len; i++) printf("%d ", ref_string[i]);
    printf("\nFrames: %d\n\n", frame_count);

    PagingSystem ps;
    paging_init(&ps, 6, frame_count);

    printf("--- FIFO ---\n");
    int fifo_faults = paging_fifo(&ps, ref_string, len, 1);
    printf("\nTotal FIFO page faults: %d (rate: %.2f%%)\n\n", fifo_faults, 100.0f * fifo_faults / len);

    printf("--- LRU ---\n");
    int lru_faults = paging_lru(&ps, ref_string, len, 1);
    printf("\nTotal LRU page faults: %d (rate: %.2f%%)\n\n", lru_faults, 100.0f * lru_faults / len);
}

static void partition_demo(void) {
    print_banner("Dynamic Partition Demo");
    PartitionManager pm;
    pm_init(&pm, 1024);

    printf("Initial state:\n");
    pm_print(&pm);

    printf("\nAllocate P1(200) with FF:\n");
    pm_allocate_ff(&pm, 1, 200);
    pm_print(&pm);

    printf("\nAllocate P2(300) with FF:\n");
    pm_allocate_ff(&pm, 2, 300);
    pm_print(&pm);

    printf("\nAllocate P3(150) with FF:\n");
    pm_allocate_ff(&pm, 3, 150);
    pm_print(&pm);

    printf("\nFree P2:\n");
    pm_free(&pm, 2);
    pm_print(&pm);

    printf("\nAllocate P4(250) with BF (should fit in 300 hole):\n");
    pm_allocate_bf(&pm, 4, 250);
    pm_print(&pm);

    pm_destroy(&pm);
}

static void run_tests(void) {
    /* Test paging */
    printf("\n========== PAGING TEST ==========\n");
    int ref1[] = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 1, 2, 0, 1, 7, 0, 1};
    int ref2[] = {1, 2, 3, 4, 2, 1, 5, 6, 2, 1, 2, 3, 7, 6, 3, 2, 1, 2, 3, 6};
    PagingSystem ps;
    paging_init(&ps, 8, 3);
    int f1 = paging_fifo(&ps, ref1, 20, 0);
    int l1 = paging_lru(&ps, ref1, 20, 0);
    printf("Test1 (3 frames): FIFO=%d LRU=%d\n", f1, l1);

    paging_init(&ps, 8, 4);
    f1 = paging_fifo(&ps, ref1, 20, 0);
    l1 = paging_lru(&ps, ref1, 20, 0);
    printf("Test1 (4 frames): FIFO=%d LRU=%d\n", f1, l1);

    paging_init(&ps, 8, 3);
    int f2 = paging_fifo(&ps, ref2, 20, 0);
    int l2 = paging_lru(&ps, ref2, 20, 0);
    printf("Test2 (3 frames): FIFO=%d LRU=%d\n", f2, l2);

    /* Test partition */
    printf("\n========== PARTITION TEST ==========\n");
    PartitionManager pm;
    pm_init(&pm, 1000);
    int a1 = pm_allocate_ff(&pm, 1, 100);
    int a2 = pm_allocate_ff(&pm, 2, 200);
    int a3 = pm_allocate_ff(&pm, 3, 150);
    printf("FF allocations: P1@%d P2@%d P3@%d\n", a1, a2, a3);
    pm_free(&pm, 2);
    int a4 = pm_allocate_bf(&pm, 4, 180);
    printf("After free P2, BF alloc P4@%d\n", a4);
    pm_print(&pm);
    pm_destroy(&pm);
}

int main(int argc, char *argv[]) {
    if (argc > 1 && strcmp(argv[1], "test") == 0) {
        run_tests();
        return 0;
    }

    int choice;
    print_banner("Memory Management Simulator");
    printf("1. Page Replacement (FIFO / LRU)\n");
    printf("2. Dynamic Partition (FF / BF)\n");
    printf("3. Run All Tests\n");
    printf("Choice: ");
    if (scanf("%d", &choice) != 1) { WARN("Invalid input"); return 1; }

    switch (choice) {
        case 1: paging_demo(); break;
        case 2: partition_demo(); break;
        case 3: run_tests(); break;
        default: WARN("Invalid choice."); return 1;
    }
    return 0;
}
