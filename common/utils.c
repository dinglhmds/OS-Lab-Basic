#include "utils.h"

int read_line(char *buf, size_t size) {
    if (!buf || size == 0) return -1;
    if (fgets(buf, size, stdin) == NULL) return -1;
    size_t len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n') buf[len - 1] = '\0';
    return 0;
}

int parse_int_list(const char *str, int *out, int max_count) {
    int count = 0;
    const char *p = str;
    while (*p && count < max_count) {
        char *end;
        long val = strtol(p, &end, 10);
        if (end == p) { p++; continue; }
        out[count++] = (int)val;
        p = end;
    }
    return count;
}

void print_table_line(int width) {
    for (int i = 0; i < width; i++) putchar('-');
    putchar('\n');
}

void print_banner(const char *title) {
    int len = (int)strlen(title);
    int total = len + 4;
    putchar('+');
    for (int i = 0; i < total; i++) putchar('-');
    printf("+\n|  %s  |\n+", title);
    for (int i = 0; i < total; i++) putchar('-');
    printf("+\n");
}
