#ifndef COMMON_H
#define COMMON_H

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>

/* Color macros for terminal output */
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_BLUE    "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN    "\033[36m"
#define COLOR_RESET   "\033[0m"

/* Error handling macros */
#define PANIC(fmt, ...) do { \
    fprintf(stderr, COLOR_RED "[PANIC] %s:%d: " fmt COLOR_RESET "\n", \
            __FILE__, __LINE__, ##__VA_ARGS__); \
    exit(EXIT_FAILURE); \
} while(0)

#define WARN(fmt, ...) do { \
    fprintf(stderr, COLOR_YELLOW "[WARN] " fmt COLOR_RESET "\n", ##__VA_ARGS__); \
} while(0)

#define INFO(fmt, ...) do { \
    printf(COLOR_CYAN "[INFO] " fmt COLOR_RESET "\n", ##__VA_ARGS__); \
} while(0)

/* Safe memory allocation */
static inline void *safe_malloc(size_t size) {
    void *p = malloc(size);
    if (!p && size > 0) PANIC("malloc(%zu) failed", size);
    return p;
}

static inline void *safe_calloc(size_t nmemb, size_t size) {
    void *p = calloc(nmemb, size);
    if (!p && nmemb > 0 && size > 0) PANIC("calloc(%zu, %zu) failed", nmemb, size);
    return p;
}

static inline void *safe_realloc(void *ptr, size_t size) {
    void *p = realloc(ptr, size);
    if (!p && size > 0) PANIC("realloc(%zu) failed", size);
    return p;
}

#endif /* COMMON_H */
