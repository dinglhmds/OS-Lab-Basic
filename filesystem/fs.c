#include "fs.h"
#include "common.h"
#include "utils.h"
#include <string.h>
#include <stdio.h>

void fs_init(FileSystem *fs) {
    fs->sb.inode_count = INODE_COUNT;
    fs->sb.block_count = BLOCK_COUNT;
    fs->sb.free_inodes = INODE_COUNT - 1; /* Reserve inode 0 for root */
    fs->sb.free_blocks = BLOCK_COUNT;
    memset(fs->inodes, 0, sizeof(fs->inodes));
    bitmap_init(&fs->bitmap);
    disk_init(&fs->disk);

    /* Setup root directory inode 0 */
    fs->inodes[0].id = 0;
    fs->inodes[0].type = 1;
    fs->inodes[0].size = 0;
    fs->inodes[0].block_count = 1;
    fs->inodes[0].blocks[0] = bitmap_alloc(&fs->bitmap, 1);
    fs->sb.free_blocks--;
    fs->current_dir_inode = 0;
}

static int find_inode_in_dir(const FileSystem *fs, int dir_inode, const char *name) {
    DirEntry entries[MAX_DIR_ENTRIES];
    int count = fs->inodes[dir_inode].size / (int)sizeof(DirEntry);
    if (count == 0) return -1;
    disk_read(&fs->disk, fs->inodes[dir_inode].blocks[0], entries, BLOCK_SIZE);
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].name, name) == 0) return entries[i].inode_id;
    }
    return -1;
}

static int add_dir_entry(FileSystem *fs, int dir_inode, const char *name, int inode_id) {
    DirEntry entries[MAX_DIR_ENTRIES];
    int count = fs->inodes[dir_inode].size / sizeof(DirEntry);
    if (count >= (int)MAX_DIR_ENTRIES) return -1;
    if (count > 0) disk_read(&fs->disk, fs->inodes[dir_inode].blocks[0], entries, BLOCK_SIZE);
    strcpy(entries[count].name, name);
    entries[count].inode_id = inode_id;
    fs->inodes[dir_inode].size += sizeof(DirEntry);
    disk_write(&fs->disk, fs->inodes[dir_inode].blocks[0], entries, BLOCK_SIZE);
    return 0;
}

static int remove_dir_entry(FileSystem *fs, int dir_inode, const char *name) {
    DirEntry entries[MAX_DIR_ENTRIES];
    int count = fs->inodes[dir_inode].size / sizeof(DirEntry);
    if (count == 0) return -1;
    disk_read(&fs->disk, fs->inodes[dir_inode].blocks[0], entries, BLOCK_SIZE);
    int idx = -1;
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].name, name) == 0) { idx = i; break; }
    }
    if (idx == -1) return -1;
    for (int i = idx; i < count - 1; i++) entries[i] = entries[i + 1];
    fs->inodes[dir_inode].size -= sizeof(DirEntry);
    disk_write(&fs->disk, fs->inodes[dir_inode].blocks[0], entries, BLOCK_SIZE);
    return 0;
}

static int alloc_inode(FileSystem *fs) {
    for (int i = 1; i < INODE_COUNT; i++) {
        if (fs->inodes[i].id == 0 && i != 0) {
            fs->inodes[i].id = i;
            fs->sb.free_inodes--;
            return i;
        }
    }
    return -1;
}

static void free_inode(FileSystem *fs, int inode_id) {
    Inode *in = &fs->inodes[inode_id];
    for (int i = 0; i < in->block_count; i++) {
        bitmap_free(&fs->bitmap, in->blocks[i], 1);
        fs->sb.free_blocks++;
    }
    memset(in, 0, sizeof(Inode));
    fs->sb.free_inodes++;
}

int fs_mkdir(FileSystem *fs, const char *name) {
    if (find_inode_in_dir(fs, fs->current_dir_inode, name) >= 0) {
        WARN("'%s' already exists", name); return -1;
    }
    int inode_id = alloc_inode(fs);
    if (inode_id < 0) { WARN("No free inodes"); return -1; }
    int blk = bitmap_alloc(&fs->bitmap, 1);
    if (blk < 0) { free_inode(fs, inode_id); WARN("No free blocks"); return -1; }
    fs->sb.free_blocks--;

    fs->inodes[inode_id].type = 1;
    fs->inodes[inode_id].size = 0;
    fs->inodes[inode_id].block_count = 1;
    fs->inodes[inode_id].blocks[0] = blk;
    add_dir_entry(fs, fs->current_dir_inode, name, inode_id);
    printf("Created directory '%s' (inode %d)\n", name, inode_id);
    return 0;
}

int fs_cd(FileSystem *fs, const char *name) {
    if (strcmp(name, "..") == 0) {
        if (fs->current_dir_inode != 0) fs->current_dir_inode = 0; /* Simplified: always go to root */
        return 0;
    }
    int inode_id = find_inode_in_dir(fs, fs->current_dir_inode, name);
    if (inode_id < 0 || fs->inodes[inode_id].type != 1) {
        WARN("'%s' is not a directory", name); return -1;
    }
    fs->current_dir_inode = inode_id;
    return 0;
}

void fs_ls(const FileSystem *fs) {
    DirEntry entries[MAX_DIR_ENTRIES];
    int count = fs->inodes[fs->current_dir_inode].size / sizeof(DirEntry);
    if (count == 0) { printf("(empty directory)\n"); return; }
    disk_read(&fs->disk, fs->inodes[fs->current_dir_inode].blocks[0], entries, BLOCK_SIZE);
    printf("%-8s %-6s %-8s\n", "Name", "Type", "Size");
    print_table_line(30);
    for (int i = 0; i < count; i++) {
        const Inode *in = &fs->inodes[entries[i].inode_id];
        printf("%-8s %-6s %-8d\n", entries[i].name,
               in->type == 1 ? "dir" : "file", in->size);
    }
}

int fs_touch(FileSystem *fs, const char *name) {
    if (find_inode_in_dir(fs, fs->current_dir_inode, name) >= 0) {
        WARN("'%s' already exists", name); return -1;
    }
    int inode_id = alloc_inode(fs);
    if (inode_id < 0) { WARN("No free inodes"); return -1; }
    fs->inodes[inode_id].type = 0;
    fs->inodes[inode_id].size = 0;
    fs->inodes[inode_id].block_count = 0;
    add_dir_entry(fs, fs->current_dir_inode, name, inode_id);
    printf("Created file '%s' (inode %d)\n", name, inode_id);
    return 0;
}

int fs_write(FileSystem *fs, const char *name, const char *data) {
    int inode_id = find_inode_in_dir(fs, fs->current_dir_inode, name);
    if (inode_id < 0 || fs->inodes[inode_id].type != 0) {
        WARN("'%s' is not a file", name); return -1;
    }
    Inode *in = &fs->inodes[inode_id];
    /* Free old blocks */
    for (int i = 0; i < in->block_count; i++) {
        bitmap_free(&fs->bitmap, in->blocks[i], 1);
        fs->sb.free_blocks++;
    }
    int len = strlen(data);
    int needed = (len + BLOCK_SIZE - 1) / BLOCK_SIZE;
    if (needed > DIRECT_BLOCKS) { WARN("File too large"); return -1; }
    for (int i = 0; i < needed; i++) {
        int blk = bitmap_alloc(&fs->bitmap, 1);
        if (blk < 0) { WARN("Disk full"); return -1; }
        fs->sb.free_blocks--;
        in->blocks[i] = blk;
        disk_write(&fs->disk, blk, data + i * BLOCK_SIZE,
                   (len - i * BLOCK_SIZE) > BLOCK_SIZE ? BLOCK_SIZE : (len - i * BLOCK_SIZE));
    }
    in->block_count = needed;
    in->size = len;
    printf("Wrote %d bytes to '%s'\n", len, name);
    return 0;
}

int fs_cat(const FileSystem *fs, const char *name) {
    int inode_id = find_inode_in_dir(fs, fs->current_dir_inode, name);
    if (inode_id < 0 || fs->inodes[inode_id].type != 0) {
        WARN("'%s' is not a file", name); return -1;
    }
    const Inode *in = &fs->inodes[inode_id];
    printf("--- Content of '%s' (%d bytes) ---\n", name, in->size);
    for (int i = 0; i < in->block_count; i++) {
        char buf[BLOCK_SIZE + 1];
        disk_read(&fs->disk, in->blocks[i], buf, BLOCK_SIZE);
        int to_print = (in->size - i * BLOCK_SIZE);
        if (to_print > BLOCK_SIZE) to_print = BLOCK_SIZE;
        buf[to_print] = '\0';
        printf("%s", buf);
    }
    printf("\n--- End ---\n");
    return 0;
}

int fs_rm(FileSystem *fs, const char *name) {
    int inode_id = find_inode_in_dir(fs, fs->current_dir_inode, name);
    if (inode_id < 0) { WARN("'%s' not found", name); return -1; }
    if (fs->inodes[inode_id].type == 1 && fs->inodes[inode_id].size > 0) {
        WARN("Directory not empty"); return -1;
    }
    remove_dir_entry(fs, fs->current_dir_inode, name);
    free_inode(fs, inode_id);
    printf("Removed '%s'\n", name);
    return 0;
}

void fs_df(const FileSystem *fs) {
    printf("Disk Usage:\n");
    printf("  Total blocks: %d\n", fs->sb.block_count);
    printf("  Free blocks:  %d\n", fs->sb.free_blocks);
    printf("  Used blocks:  %d\n", fs->sb.block_count - fs->sb.free_blocks);
    printf("  Total inodes: %d\n", fs->sb.inode_count);
    printf("  Free inodes:  %d\n", fs->sb.free_inodes);
}
