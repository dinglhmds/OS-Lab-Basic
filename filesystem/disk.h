#ifndef DISK_H
#define DISK_H

#include "common.h"

#define BLOCK_SIZE 512
#define BLOCK_COUNT 1024

typedef struct {
    uint8_t data[BLOCK_COUNT][BLOCK_SIZE];
    int block_count;
} Disk;

void disk_init(Disk *disk);
void disk_write(Disk *disk, int block_id, const void *data, int size);
void disk_read(const Disk *disk, int block_id, void *data, int size);
void disk_clear(Disk *disk, int block_id);

#endif /* DISK_H */
