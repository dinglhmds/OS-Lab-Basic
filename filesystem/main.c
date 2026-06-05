#include "fs.h"
#include "common.h"
#include "utils.h"
#include <string.h>
#include <stdio.h>

static void run_tests(void) {
    print_banner("Filesystem Test");
    FileSystem fs;
    fs_init(&fs);

    fs_touch(&fs, "hello.txt");
    fs_write(&fs, "hello.txt", "Hello, OS Course Design!");
    fs_cat(&fs, "hello.txt");

    fs_mkdir(&fs, "docs");
    fs_cd(&fs, "docs");
    fs_touch(&fs, "notes.txt");
    fs_write(&fs, "notes.txt", "This is a note inside docs directory.");
    fs_ls(&fs);
    fs_cat(&fs, "notes.txt");
    fs_cd(&fs, "..");

    printf("\nRoot directory:\n");
    fs_ls(&fs);
    fs_df(&fs);

    fs_rm(&fs, "hello.txt");
    printf("\nAfter rm hello.txt:\n");
    fs_ls(&fs);
    fs_df(&fs);
}

int main(int argc, char *argv[]) {
    if (argc > 1 && strcmp(argv[1], "test") == 0) {
        run_tests();
        return 0;
    }

    FileSystem fs;
    fs_init(&fs);
    char cmd[64], arg1[32];

    print_banner("Simple File System");
    printf("Commands: touch <name>, write <name> <data>, cat <name>,\n");
    printf("          mkdir <name>, cd <name>, ls, rm <name>, df, quit\n\n");

    while (1) {
        printf("fs> ");
        if (read_line(cmd, sizeof(cmd)) < 0) break;
        if (strcmp(cmd, "quit") == 0 || strcmp(cmd, "exit") == 0) break;
        if (strcmp(cmd, "ls") == 0) { fs_ls(&fs); continue; }
        if (strcmp(cmd, "df") == 0) { fs_df(&fs); continue; }

        if (sscanf(cmd, "touch %31s", arg1) == 1) {
            fs_touch(&fs, arg1);
        } else if (sscanf(cmd, "mkdir %31s", arg1) == 1) {
            fs_mkdir(&fs, arg1);
        } else if (sscanf(cmd, "cd %31s", arg1) == 1) {
            fs_cd(&fs, arg1);
        } else if (sscanf(cmd, "rm %31s", arg1) == 1) {
            fs_rm(&fs, arg1);
        } else if (sscanf(cmd, "cat %31s", arg1) == 1) {
            fs_cat(&fs, arg1);
        } else if (strncmp(cmd, "write ", 6) == 0) {
            char *name = cmd + 6;
            char *data = strchr(name, ' ');
            if (data) { *data = '\0'; data++; }
            else { data = ""; }
            fs_write(&fs, name, data);
        } else {
            printf("Unknown command.\n");
        }
    }
    return 0;
}
