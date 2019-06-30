#include <common.h>
#include <klib.h>
#include <devices.h>

struct ext2{
  uint32_t size;
  uint32_t inode_num;
  uint32_t block_num;
  char inode_used[1024];
  char block_used[1024];
  device_t *dev;
}ext2_t;

int ext2_init(filesystem_t* fs, const char* name, device_t *dev){
    ezt2_t* ext2fs=(ezt2_t*)fs->myfs;
    memset(ext2fs,0,sizeof(ext2_t));
    
}

