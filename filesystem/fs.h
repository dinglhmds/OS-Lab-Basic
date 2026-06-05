#ifndef FS_H
#define FS_H

#include "disk.h"
#include "bitmap.h"

#define INODE_COUNT 128
#define NAME_LEN 28
#define DIRECT_BLOCKS 10
#define MAX_DIR_ENTRIES (BLOCK_SIZE / sizeof(DirEntry))

typedef struct {
    int inode_count;
    int block_count;
    int free_inodes;
    int free_blocks;
} SuperBlock;

typedef struct {
    int id;
    int type; /* 0=file, 1=dir */
    int size;
    int blocks[DIRECT_BLOCKS];
    int block_count;
} Inode;

typedef struct {
    char name[NAME_LEN];
    int inode_id;
} DirEntry;

typedef struct {
    SuperBlock sb;
    Inode inodes[INODE_COUNT];
    Bitmap bitmap;
    Disk disk;
    int current_dir_inode; /* Inode of current directory */
} FileSystem;

/* Lifecycle */
void fs_init(FileSystem *fs);

/* Directory operations */
int fs_mkdir(FileSystem *fs, const char *name);
int fs_cd(FileSystem *fs, const char *name);
void fs_ls(const FileSystem *fs);

/* File operations */
int fs_touch(FileSystem *fs, const char *name);
int fs_write(FileSystem *fs, const char *name, const char *data);
int fs_cat(const FileSystem *fs, const char *name);
int fs_rm(FileSystem *fs, const char *name);

/* Utility */
void fs_df(const FileSystem *fs);

#endif /* FS_H */
