#ifndef __EXT2_H__
#define __EXT2_H__

#include <vfs.h>
#include <common.h>
#include <klib.h>


typedef struct ext2_dit{
    char name[20];
    int inode[16];
    int dir[16];
    int inode_size;
    int dir_size;
    int type;
}ext2_dir_t;

int ext2_init(filesystem_t* fs, const char* name, device_t *dev);
int ext2_alloc(filesystem_t *fs, int inode);
int ext2_free(filesystem_t *fs, int inode);
int ext2_lookup(filesystem_t *fs, const char *path, int flags);
int ext2_read(filesystem_t *fs, int inode, uint64_t offset, int len, char *buf);
int ext2_write(filesystem_t *fs, int inode, uint64_t offset, int len, char *buf);
int ext2_delete(filesystem_t *fs, int inode);
int ext2_inode();
int ext2_mkdir();
int ext2_rmdir();
int ext2_find_file();
int ext2_read_file();
int ext2_write_file();

#endif