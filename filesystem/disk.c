#include "disk.h"
#include <string.h>

void disk_init(Disk *disk) {
    disk->block_count = BLOCK_COUNT;
    memset(disk->data, 0, sizeof(disk->data));
}

void disk_write(Disk *disk, int block_id, const void *data, int size) {
    if (block_id < 0 || block_id >= BLOCK_COUNT) return;
    if (size > BLOCK_SIZE) size = BLOCK_SIZE;
    memcpy(disk->data[block_id], data, size);
}

void disk_read(const Disk *disk, int block_id, void *data, int size) {
    if (block_id < 0 || block_id >= BLOCK_COUNT) return;
    if (size > BLOCK_SIZE) size = BLOCK_SIZE;
    memcpy(data, disk->data[block_id], size);
}

void disk_clear(Disk *disk, int block_id) {
    if (block_id < 0 || block_id >= BLOCK_COUNT) return;
    memset(disk->data[block_id], 0, BLOCK_SIZE);
}
