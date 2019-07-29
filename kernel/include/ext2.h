#ifndef __EXT2_H__
#define __EXT2_H__

#include <devices.h>
#include <common.h>
#include <klib.h>

typedef struct ext2{
	uint32_t size;
	uint32_t block_num;
	uint32_t block_size;
	char block_used[1024];
	device_t *dev;
	char fsname[80];
}ext2_t;

int ext2_init(filesystem_t* fs, const char* name, device_t *dev);
int ext2_alloc(filesystem_t *fs, int inode);
int ext2_free(filesystem_t *fs, int inode);
int ext2_lookup(filesystem_t *fs, const char *path, int flags);
int ext2_read(filesystem_t *fs, int inode, uint32_t offset, int len, char *buf);
int ext2_write(filesystem_t *fs, int inode, uint32_t offset, int len, char *buf);
int ext2_delete(filesystem_t *fs, int inode);

#endif