#include "paging.h"
#include "common.h"
#include <stdio.h>

void paging_init(PagingSystem *ps, int page_count, int frame_count) {
    ps->page_count = page_count;
    ps->frame_count = frame_count;
    paging_reset(ps);
}

void paging_reset(PagingSystem *ps) {
    for (int i = 0; i < MAX_PAGES; i++) {
        ps->pages[i] = (PageEntry){i, -1, 0, -1, -1};
    }
    for (int i = 0; i < MAX_FRAMES; i++) {
        ps->fifo_queue[i] = -1;
    }
    ps->fifo_head = 0;
    ps->fifo_tail = 0;
    ps->current_time = 0;
}

void paging_print_state(const PagingSystem *ps) {
    printf("Page Table (V=Valid, F=Frame, L=Load, A=LastAccess):\n");
    printf("%-6s %-4s %-4s %-6s %-10s\n", "Page", "V", "F", "Load", "Access");
    for (int i = 0; i < ps->page_count; i++) {
        if (ps->pages[i].valid) {
            printf("%-6d %-4d %-4d %-6d %-10d\n",
                   ps->pages[i].page_id, ps->pages[i].valid,
                   ps->pages[i].frame_id, ps->pages[i].load_time,
                   ps->pages[i].last_access);
        }
    }
}

int paging_fifo(PagingSystem *ps, int ref_string[], int len, int verbose) {
    paging_reset(ps);
    int faults = 0;
    int occupied = 0;

    for (int i = 0; i < len; i++) {
        int page = ref_string[i];
        ps->current_time++;
        if (verbose) printf("\nAccess page %d: ", page);

        if (ps->pages[page].valid) {
            if (verbose) printf("HIT\n");
            ps->pages[page].last_access = ps->current_time;
            continue;
        }

        faults++;
        if (verbose) printf("MISS (fault #%d)\n", faults);

        if (occupied < ps->frame_count) {
            int frame = occupied;
            ps->pages[page].frame_id = frame;
            ps->pages[page].valid = 1;
            ps->pages[page].load_time = ps->current_time;
            ps->pages[page].last_access = ps->current_time;
            ps->fifo_queue[ps->fifo_tail] = page;
            ps->fifo_tail = (ps->fifo_tail + 1) % ps->frame_count;
            occupied++;
        } else {
            int victim = ps->fifo_queue[ps->fifo_head];
            ps->fifo_head = (ps->fifo_head + 1) % ps->frame_count;
            int frame = ps->pages[victim].frame_id;
            ps->pages[victim].valid = 0;
            ps->pages[victim].frame_id = -1;
            ps->pages[page].frame_id = frame;
            ps->pages[page].valid = 1;
            ps->pages[page].load_time = ps->current_time;
            ps->pages[page].last_access = ps->current_time;
            ps->fifo_queue[ps->fifo_tail] = page;
            ps->fifo_tail = (ps->fifo_tail + 1) % ps->frame_count;
            if (verbose) printf("  Evict page %d, load page %d into frame %d\n", victim, page, frame);
        }
        if (verbose) paging_print_state(ps);
    }
    return faults;
}

int paging_lru(PagingSystem *ps, int ref_string[], int len, int verbose) {
    paging_reset(ps);
    int faults = 0;
    int occupied = 0;

    for (int i = 0; i < len; i++) {
        int page = ref_string[i];
        ps->current_time++;
        if (verbose) printf("\nAccess page %d: ", page);

        if (ps->pages[page].valid) {
            if (verbose) printf("HIT\n");
            ps->pages[page].last_access = ps->current_time;
            continue;
        }

        faults++;
        if (verbose) printf("MISS (fault #%d)\n", faults);

        if (occupied < ps->frame_count) {
            int frame = occupied;
            ps->pages[page].frame_id = frame;
            ps->pages[page].valid = 1;
            ps->pages[page].load_time = ps->current_time;
            ps->pages[page].last_access = ps->current_time;
            occupied++;
        } else {
            int victim = -1;
            int min_access = ps->current_time + 1;
            for (int p = 0; p < ps->page_count; p++) {
                if (ps->pages[p].valid && ps->pages[p].last_access < min_access) {
                    min_access = ps->pages[p].last_access;
                    victim = p;
                }
            }
            int frame = ps->pages[victim].frame_id;
            ps->pages[victim].valid = 0;
            ps->pages[victim].frame_id = -1;
            ps->pages[page].frame_id = frame;
            ps->pages[page].valid = 1;
            ps->pages[page].load_time = ps->current_time;
            ps->pages[page].last_access = ps->current_time;
            if (verbose) printf("  Evict page %d, load page %d into frame %d\n", victim, page, frame);
        }
        if (verbose) paging_print_state(ps);
    }
    return faults;
}
