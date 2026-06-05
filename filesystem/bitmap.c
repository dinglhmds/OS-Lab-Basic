#include "bitmap.h"
#include <stdio.h>

void bitmap_init(Bitmap *bm) {
    memset(bm->bits, 0, sizeof(bm->bits));
}

static void set_bit(Bitmap *bm, int idx, int val) {
    int byte = idx / 8;
    int bit = idx % 8;
    if (val) bm->bits[byte] |= (1 << bit);
    else bm->bits[byte] &= ~(1 << bit);
}

int bitmap_test(const Bitmap *bm, int idx) {
    int byte = idx / 8;
    int bit = idx % 8;
    return (bm->bits[byte] >> bit) & 1;
}

int bitmap_alloc(Bitmap *bm, int count) {
    int start = -1;
    int found = 0;
    for (int i = 0; i < BITMAP_BITS && found < count; i++) {
        if (!bitmap_test(bm, i)) {
            if (start == -1) start = i;
            found++;
        } else {
            start = -1;
            found = 0;
        }
    }
    if (found < count) return -1;
    for (int i = start; i < start + count; i++) set_bit(bm, i, 1);
    return start;
}

void bitmap_free(Bitmap *bm, int start, int count) {
    for (int i = start; i < start + count && i < BITMAP_BITS; i++) {
        set_bit(bm, i, 0);
    }
}

void bitmap_print(const Bitmap *bm, int max_bits) {
    printf("Bitmap (0=free, 1=used):\n");
    for (int i = 0; i < max_bits && i < BITMAP_BITS; i++) {
        printf("%d", bitmap_test(bm, i));
        if ((i + 1) % 64 == 0) printf("\n");
    }
    if (max_bits % 64) printf("\n");
}
