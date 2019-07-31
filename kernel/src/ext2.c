#include<ext2.h>

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

int ext2_alloc(filesystem_t *fs, int inode){
    ext2_t* ext2fs=(ext2_t*)fs->myfs;
    if(ext2fs->block_used[inode])
        return -1;
    ext2fs->block_used[inode]=1;
    return 1;
}

int ext2_free(filesystem_t *fs, int inode){
    ext2_t* ext2fs=(ext2_t*)fs->myfs;
    ext2fs->block_used[inode]=0;
    return 1;
}

int ext2_lookup(filesystem_t *fs, const char *path, int flags){
    return 0;
}

int ext2_read(filesystem_t *fs, int inode, uint64_t offset, int len, char *buf){
    ext2_t* ext2fs=(ext2_t*)fs->myfs;
    if(ext2fs->block_used[inode]==0){
        *buf='\0';
        return 0;
    }
    int start=inode*4096+offset;
    if(len+offset>ext2fs->block_size)
        len=ext2fs->block_size-offset;
    ext2fs->dev->ops->read(ext2fs->dev,start,buf,len);
    return len;
}

int ext2_write(filesystem_t *fs, int inode, uint64_t offset, int len, char *buf){
    ext2_t* ext2fs=(ext2_t*)fs;
    /*for(int idx=inode*4,i=0;i<len;i++,idx++){
        ext2fs->block_used[idx]=1;
    }*/
    ext2_alloc(fs, inode);
    int start=inode*4096+offset;
    if(len+offset>ext2fs->block_size)
        len=ext2fs->block_size-offset;
    printf("nms\n");
    //ext2fs->dev=dev_lookup("ramdisk0");
    ext2fs->dev->ops->write(ext2fs->dev,start,buf,len);
    return len;
}

int ext2_delete(filesystem_t *fs, int inode){
    ext2_t* ext2fs=(ext2_t*)fs->myfs;
    ext2_free(fs, inode);
    const char buf[4096]={'\0'};
    int start=inode*4096;
    ext2fs->dev->ops->write(ext2fs->dev,start,buf,ext2fs->block_size);
    return 1;
}

int ext2_build(int fa){
    printf("nmsl\n");
    int id=vinode_addfile(fa, FILE, "a.txt", EXT2, &mount_table[r0]);
    char buf[80]={"hello\n"};
    ext2_write(&mount_table[r0],id,0,strlen(buf),buf);
    id=vinode_addfile(fa, FILE, "hello.cpp", EXT2, &mount_table[r0]);
    //char buf2[200]={"#include<iostream>\nusing namespace std;\nint main(){\n    cout<<\"hello world\"<<endl;\n    return 0;\n}\n"};
    //ext2_write(&mount_table[r0],id,0,strlen(buf2),buf2);
    printf("%d\n",id);
    return fa;
}

int ext2_clear(filesystem_t *fs, int inode){
    ext2_t* ext2fs=(ext2_t*)fs->myfs;
    const char buf[4096]={'\0'};
    int start=inode*4096;
    ext2fs->dev->ops->write(ext2fs->dev,start,buf,ext2fs->block_size);
    return 1;
}



