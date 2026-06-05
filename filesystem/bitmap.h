#ifndef BITMAP_H
#define BITMAP_H

#include "common.h"

#define BITMAP_BITS 1024

typedef struct {
    uint8_t bits[BITMAP_BITS / 8];
} Bitmap;

void bitmap_init(Bitmap *bm);
int bitmap_alloc(Bitmap *bm, int count);
void bitmap_free(Bitmap *bm, int start, int count);
int bitmap_test(const Bitmap *bm, int idx);
void bitmap_print(const Bitmap *bm, int max_bits);

#endif /* BITMAP_H */
