#ifndef UTILS_H
#define UTILS_H

#include "common.h"

/* Read a line from stdin into buffer, returns 0 on success, -1 on EOF/error */
int read_line(char *buf, size_t size);

/* Parse a comma/space separated list of integers */
int parse_int_list(const char *str, int *out, int max_count);

/* Print a horizontal table line */
void print_table_line(int width);

/* Print a centered title banner */
void print_banner(const char *title);

#endif /* UTILS_H */
