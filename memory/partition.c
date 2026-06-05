#include "partition.h"
#include "common.h"
#include "utils.h"
#include <stdio.h>

void pm_init(PartitionManager *pm, int total_size) {
    pm->total_size = total_size;
    pm->head = safe_malloc(sizeof(Partition));
    pm->head->start = 0;
    pm->head->size = total_size;
    pm->head->is_free = 1;
    pm->head->pid = -1;
    pm->head->next = NULL;
}

void pm_destroy(PartitionManager *pm) {
    Partition *cur = pm->head;
    while (cur) {
        Partition *next = cur->next;
        free(cur);
        cur = next;
    }
    pm->head = NULL;
}

static void merge_free(PartitionManager *pm) {
    Partition *cur = pm->head;
    while (cur && cur->next) {
        if (cur->is_free && cur->next->is_free) {
            Partition *next = cur->next;
            cur->size += next->size;
            cur->next = next->next;
            free(next);
        } else {
            cur = cur->next;
        }
    }
}

int pm_allocate_ff(PartitionManager *pm, int pid, int size) {
    Partition *cur = pm->head;
    while (cur) {
        if (cur->is_free && cur->size >= size) {
            if (cur->size > size) {
                Partition *new_part = safe_malloc(sizeof(Partition));
                new_part->start = cur->start + size;
                new_part->size = cur->size - size;
                new_part->is_free = 1;
                new_part->pid = -1;
                new_part->next = cur->next;
                cur->next = new_part;
                cur->size = size;
            }
            cur->is_free = 0;
            cur->pid = pid;
            return cur->start;
        }
        cur = cur->next;
    }
    return -1; /* Allocation failed */
}

int pm_allocate_bf(PartitionManager *pm, int pid, int size) {
    Partition *best = NULL;
    Partition *cur = pm->head;
    while (cur) {
        if (cur->is_free && cur->size >= size) {
            if (!best || cur->size < best->size) {
                best = cur;
            }
        }
        cur = cur->next;
    }
    if (!best) return -1;
    if (best->size > size) {
        Partition *new_part = safe_malloc(sizeof(Partition));
        new_part->start = best->start + size;
        new_part->size = best->size - size;
        new_part->is_free = 1;
        new_part->pid = -1;
        new_part->next = best->next;
        best->next = new_part;
        best->size = size;
    }
    best->is_free = 0;
    best->pid = pid;
    return best->start;
}

void pm_free(PartitionManager *pm, int pid) {
    Partition *cur = pm->head;
    while (cur) {
        if (!cur->is_free && cur->pid == pid) {
            cur->is_free = 1;
            cur->pid = -1;
            merge_free(pm);
            return;
        }
        cur = cur->next;
    }
    WARN("Process %d not found in memory", pid);
}

void pm_print(const PartitionManager *pm) {
    printf("%-8s %-8s %-8s %-8s\n", "Start", "Size", "Status", "PID");
    print_table_line(40);
    Partition *cur = pm->head;
    while (cur) {
        printf("%-8d %-8d %-8s %-8d\n",
               cur->start, cur->size,
               cur->is_free ? "Free" : "Used",
               cur->is_free ? -1 : cur->pid);
        cur = cur->next;
    }
    print_table_line(40);
}

void pm_print_ascii(const PartitionManager *pm) {
    printf("\nMemory Map [0 - %d]:\n", pm->total_size);
    int scale = pm->total_size / 64;
    if (scale < 1) scale = 1;
    Partition *cur = pm->head;
    while (cur) {
        int blocks = cur->size / scale;
        if (blocks < 1) blocks = 1;
        char c = cur->is_free ? '.' : ('0' + (cur->pid % 10));
        for (int i = 0; i < blocks; i++) putchar(c);
    }
    printf("\n  . = Free  [0-9] = Process ID (mod 10)\n");
}
