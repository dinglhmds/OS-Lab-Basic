#ifndef PAGING_H
#define PAGING_H

#define MAX_PAGES 32
#define MAX_FRAMES 8
#define MAX_REF_LEN 64

typedef struct {
    int page_id;
    int frame_id;
    int valid;
    int load_time;
    int last_access;
} PageEntry;

typedef struct {
    PageEntry pages[MAX_PAGES];
    int page_count;
    int frame_count;
    int fifo_queue[MAX_FRAMES];
    int fifo_head;
    int fifo_tail;
    int current_time;
} PagingSystem;

void paging_init(PagingSystem *ps, int page_count, int frame_count);
void paging_reset(PagingSystem *ps);
int paging_fifo(PagingSystem *ps, int ref_string[], int len, int verbose);
int paging_lru(PagingSystem *ps, int ref_string[], int len, int verbose);
void paging_print_state(const PagingSystem *ps);

#endif /* PAGING_H */
