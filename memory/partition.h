#ifndef PARTITION_H
#define PARTITION_H

#define MEM_SIZE 1024

typedef struct Partition {
    int start;
    int size;
    int is_free;
    int pid; /* -1 if free */
    struct Partition *next;
} Partition;

typedef struct {
    Partition *head;
    int total_size;
} PartitionManager;

void pm_init(PartitionManager *pm, int total_size);
void pm_destroy(PartitionManager *pm);

/* Allocation algorithms */
int pm_allocate_ff(PartitionManager *pm, int pid, int size);
int pm_allocate_bf(PartitionManager *pm, int pid, int size);

/* Deallocation */
void pm_free(PartitionManager *pm, int pid);

/* Display */
void pm_print(const PartitionManager *pm);
void pm_print_ascii(const PartitionManager *pm);

#endif /* PARTITION_H */
