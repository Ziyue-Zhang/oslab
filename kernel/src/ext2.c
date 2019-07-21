#include <common.h>
#include <klib.h>
#include <devices.h>

typedef struct ext2{
  uint32_t size;
  uint32_t block_num;
  uint32_t block_size;
  char block_used[1024];
  char block_len[256];
  device_t *dev;
  char fsname[80];
}ext2_t;

int ext2_init(filesystem_t* fs, const char* name, device_t *dev){
    ext2_t* ext2fs=(ext2_t*)fs->myfs;
    memset(ext2fs,0,sizeof(ext2_t));
    memset(ext2fs->block_used,0,sizeof(ext2fs->block_used));
    ext2fs->size=4096*1024;
    ext2fs->block_num=1024;
    ext2fs->block_size=4096;
    strcpy(ext2fs->fsname,name);
    ext2fs->dev=dev;
    return 1;
}

int ext2_read(filesystem_t *fs, int inode, uint32_t offset, int len, char *buf){
    ext2_t* ext2fs=(ext2_t*)fs->myfs;
    if(ext2fs->block_used[inode]==0){
        *buf='\0';
        return 1;
    }
    int start=inode*4096+offset;
    if(len+offset>ext2fs->block_size)
        len=ext2fs->block_size-offset;
    ext2fs->dev->ops->read(ext2fs->dev,start,buf,len);
    return 1;
}

int ext2_write(filesystem_t *fs, int inode, uint32_t offset, int len, char *buf){
    ext2_t* ext2fs=(ext2_t*)fs->myfs;
    /*for(int idx=inode*4,i=0;i<len;i++,idx++){
        ext2fs->block_used[idx]=1;
    }*/
    ext2fs->block_used[inode]=1;
    int start=inode*4096+offset;
    if(len+offset>ext2fs->block_size)
        len=ext2fs->block_size-offset;
    ext2fs->dev->ops->write(ext2fs->dev,start,buf,len);
    return 1;
}

int ext2_delete(filesystem_t *fs, int inode){
    ext2_t* ext2fs=(ext2_t*)fs->myfs;
    ext2fs->block_used[inode]=0;
    char buf[4096]={'\0'};
    int start=inode*4096;
    ext2fs->dev->ops->write(ext2fs->dev,start,buf,block_size-offset);
    return 1;
}

