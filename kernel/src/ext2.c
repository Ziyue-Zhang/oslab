#include <common.h>
#include <klib.h>
#include <devices.h>

struct ext2{
  uint32_t size;
  uint32_t block_num;
  uint32_t block_size;
  char block_used[1024];
  char block_len[128];
  device_t *dev;
  char fsname[80];
}ext2_t;

int ext2_init(filesystem_t* fs, const char* name, device_t *dev){
    ext2_t* ext2fs;
    ext2fs=(ext2_t*)fs->myfs;
    memset(ext2fs,0,sizeof(ext2_t));
    ext2fs->size=4096*1024;
    ext2fs->block_num=1024;
    ext2fs->block_size=4096;
    strcpy(ext2fs->fsname,name);
    ext2fs->dev=dev;
}

int ext_read(filesystem_t *fs, int inode, int lne, char *buf){
    ext2_t* ext2fs=(ext2_t*)fs->myfs;
    return 1;
}

int ext_read(filesystem_t *fs, int inode, int lne, char *buf){
    ext2_t* ext2fs=(ext2_t*)fs->myfs;
    return 1;
}

int ext_delete(filesystem_t *fs, int inode, int lne, char *buf){
    ext2_t* ext2fs=(ext2_t*)fs->myfs;
    return 1;
}

