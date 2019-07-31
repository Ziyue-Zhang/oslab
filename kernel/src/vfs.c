#include<vfs.h>

void vinode_free(int idx){
    vinode[idx].type=EMPTY;
}
int vinode_alloc(int type){
    for(int i=0;i<VINODE_SIZE;i++){
        if(vinode[i].type==EMPTY){
            vinode[i].type=type;
            return i;
        }
    }
    return -1;
}
int vinode_setroot(){
    memset(vinode,0,sizeof(vinode));
    int id=vinode_alloc(DIR);
    strcpy(vinode[id].name,"/");
    strcpy(vinode[id].path,"/");
    
    int dot=vinode_alloc(DIR);
    int dotdot=vinode_alloc(DIR);
    strcpy(vinode[dot].name,".");
    strcpy(vinode[dot].path,"/");
    strcpy(vinode[dotdot].name,"..");
    strcpy(vinode[dotdot].path,"/");

    vinode[id].dot=-1;
    vinode[id].dotdot=-1;
    vinode[id].nxt=-1;
    vinode[id].son=dot;
    vinode[id].link_inode=id;
    vinode[id].link_count=1;
    vinode[id].filesystem=VFS;
    vinode[id].fs=NULL;

    vinode[dot].dot=dot;
    vinode[dot].dotdot=dotdot;
    vinode[dot].nxt=dotdot;
    vinode[dot].son=id;
    vinode[dot].link_inode=dot;
    vinode[dot].link_count=1;
    vinode[dot].filesystem=VFS;
    vinode[dot].fs=NULL;

    vinode[dotdot].dot=dot;
    vinode[dotdot].dotdot=dotdot;
    vinode[dotdot].nxt=-1;
    vinode[dotdot].son=id;
    vinode[dotdot].link_inode=dotdot;
    vinode[dotdot].link_count=1;
    vinode[dotdot].filesystem=VFS;
    vinode[dotdot].fs=NULL;
    return id;
}
void vinode_setdot(int this_id, int dot, int dotdot, int fstype, filesystem_t* fs){
    strcpy(vinode[dot].name,".");
    strcpy(vinode[dot].path,vinode[this_id].path);
    //strcat(vinode[dot].path,".");
    vinode[dot].dot=dot;
    vinode[dot].dotdot=dotdot;
    vinode[dot].nxt=dotdot;
    vinode[dot].son=this_id;
    vinode[dot].link_inode=dot;
    vinode[dot].link_count=1;
    vinode[dot].filesystem=fstype;
    vinode[dot].fs=fs;
}
void vinode_setdotdot(int fa_id, int dot, int dotdot, int fstype, filesystem_t* fs){
    strcpy(vinode[dotdot].name,"..");
    strcpy(vinode[dotdot].path,vinode[fa_id].path);
    //strcat(vinode[dotdot].path,"..");
    vinode[dotdot].dot=dot;
    vinode[dotdot].dotdot=dotdot;
    vinode[dotdot].nxt=-1;
    vinode[dotdot].son=fa_id;
    vinode[dotdot].link_inode=dotdot;
    vinode[dotdot].link_count=1;
    vinode[dotdot].filesystem=fstype;
    vinode[dotdot].fs=fs;
}
void vinode_setdir(int id, int dot, int dotdot, char *name, int fstype, filesystem_t* fs){
    strcpy(vinode[id].name,name);
    strcpy(vinode[id].path,vinode[dot].path);
    int len=strlen(vinode[id].path);
    vinode[id].path[len-1]='\0';
    strcat(vinode[id].path,name);
    strcat(vinode[id].path,"/");
    vinode[id].dot=dot;
    vinode[id].dotdot=dotdot;
    vinode[id].nxt=-1;
    vinode[id].son=-1;
    vinode[id].link_inode=id;
    vinode[id].link_count=1;
    vinode[id].filesystem=fstype;
    vinode[id].fs=fs;  
}
void vinode_setfile(int id, int dot, int dotdot, char *name, int fstype, filesystem_t* fs){
    strcpy(vinode[id].name,name);
    strcpy(vinode[id].path,vinode[dot].path);
    int len=strlen(vinode[id].path);
    vinode[id].path[len-1]='\0';
    strcat(vinode[id].path,name);
    vinode[id].dot=dot;
    vinode[id].dotdot=dotdot;
    vinode[id].nxt=-1;
    vinode[id].son=-1;
    vinode[id].link_inode=id;
    vinode[id].link_count=1;
    vinode[id].filesystem=fstype;
    vinode[id].fs=fs;  
}

int vinode_adddot(){
    int dot=vinode_alloc(DIR);
    return dot;
}
int vinode_adddotdot(){
    int dotdot=vinode_alloc(DIR);
    return dotdot;
}
int vinode_adddir(int fa, int type, char *name, int filesystem, filesystem_t *fs){
    int id=vinode_alloc(type);
    int p=vinode[fa].son;
    int dot=p;
    int dotdot=vinode[p].nxt;
    for(;vinode[p].nxt!=-1;p=vinode[p].nxt);
    
    vinode[p].nxt=id;
    vinode_setdir(id,dot,dotdot,name,filesystem,fs);
    
    dot=vinode_adddot();
    dotdot=vinode_adddotdot();
    vinode[id].son=dot;
    vinode_setdot(id,dot,dotdot,filesystem,fs);
    vinode_setdotdot(fa,dot,dotdot,filesystem,fs);
    return id;
}
int vinode_addfile(int fa, int type, char *name, int filesystem, filesystem_t *fs){
    int id=vinode_alloc(type);
    int p=vinode[fa].son;
    int dot=p;
    int dotdot=vinode[p].nxt;
    for(;vinode[p].nxt!=-1;p=vinode[p].nxt);
    vinode[p].nxt=id;
    vinode_setfile(id,dot,dotdot,name,filesystem,fs);
    return id;
}
int vinode_delfile(int fa, int this){
    int p=vinode[fa].son;
    if(this==p){
        vinode[fa].son=vinode[p].nxt;
        vinode_free(this);
    }
    else{
        for(;vinode[p].nxt!=this;p=vinode[p].nxt);
        vinode[p].nxt=vinode[this].nxt;
        vinode_free(this);
    }
    return 1;
}
int vinode_deldir(int fa, int this){
    int p=vinode[fa].son;
    if(this==p){
        vinode[fa].son=vinode[p].nxt;
        for(int i=vinode[this].son;i!=-1;i=vinode[i].nxt){
            if(vinode[i].type==DIR){
                vinode_deldir(this,i);
            }
            else{
                vinode_delfile(this,i);
            }
        }
        vinode_free(this);
    }
    else{
        for(;vinode[p].nxt!=this;p=vinode[p].nxt);
        vinode[p].nxt=vinode[this].nxt;
        for(int i=vinode[this].son;i!=-1;i=vinode[i].nxt){
            if(vinode[i].type==DIR){
                vinode_deldir(this,i);
            }
            else{
                vinode_delfile(this,i);
            }
        }
        vinode_free(this);
    }
    return 1;
}
int vinode_find(char *path){
    if(strlen(path)==1&&path[0]=='/')
        return 0;
    int len=strlen(path);
    int id=0;
    for(int i=0;i<len;i++){
        int j=i;
        if(path[j]=='/'){
            i++;
            j++;
        }
        char name[80];
        int k=0;
        for(;path[j]!='\0'&&path[j]!='/';k++,j++)
            name[k]=path[j];
        /*if(path[j]=='/'){
            name[k]='/';
            k++;
        }*/
        name[k]='\0';
        i=j;
        //printf("%s\n",name);
        id=vinode[id].son;
        if(id==0)
            id=vinode[id].son;
        while(id!=-1){
            if(strcmp(vinode[id].name,name)==0)
                break;
            id=vinode[id].nxt;
        }
        if(id==-1)
            return -1;
        //printf("%s\n",name);
        while(vinode[id].type==LINK){
            id=vinode[id].link_inode;
        }
    }
    return id;
}
int vinode_lookup(char *path){
    int len=strlen(path);
    int id=0;
    for(int i=0;i<len;i++){
        int j=i;
        if(path[j]=='/'){
            i++;
            j++;
        }
        char name[80];
        int k=0;
        for(;path[j]!='\0'&&path[j]!='/';k++,j++)
            name[k]=path[j];
        if(path[j]=='/'){
            name[k]='/';
            k++;
        }
        name[k]='\0';
        i=j;
        int son_id=vinode[id].son;
        while(son_id!=-1){
            if(strcmp(vinode[son_id].name,name)==0)
                break;
            son_id=vinode[son_id].nxt;
        }
        if(son_id==-1 && vinode[id].filesystem!=VFS)
            return vinode[id].fs->ops->lookup(vinode[id].fs,path+i,RD_ONLY);
        else if(son_id==-1 && vinode[id].filesystem==VFS)
            return -1;
        else{
            while(vinode[son_id].type==LINK){
                son_id=vinode[son_id].link_inode;
            }
            id=son_id;  
        }      
        //printf("%s\n",name);
    }
    return id;
}

int fd_alloc(){
    for(int fd=0;fd<FILE_SIZE;fd++){
        if(fildes[fd].refcnt==0)
            return fd;
    }
    return -1;
}
int fd_free(int fd){
    fildes[fd].refcnt=0;
    return 0;
}
int fd_open(int inode){
    int fd=fd_alloc();
    if(fd==-1)
        return -1;
    fildes[fd].refcnt++;
    fildes[fd].inode=inode;
    fildes[fd].offset=0;
    return fd;
}

void vfs_init(){
    memset(vinode,0,sizeof(vinode));
    memset(fildes,0,sizeof(fildes));
    memset(mount_table,0,sizeof(mount_table));
    vinode_setroot();
}

int vfs_lookup(char *path){
    return vinode_find(path);
}
int vfs_access(const char *path, int mode){
    char temp[200];
    strcpy(temp,path);
    int id=vinode_find(temp);
    if(id==-1)
      return -1;
    if(vinode[id].mode==mode)
      return id;
    return -1;
}
int vfs_mount(const char *path, filesystem_t *fs){
    return 0;
}
int vfs_unmount(const char *path){
    return 0;
}
int vfs_mkdir(const char *path){
    char temp[200];
    strcpy(temp, path);
    int fa=vinode_find(temp);
    int id=vinode_adddir(fa,DIR,NULL,VFS,NULL);
    return id;
}
int vfs_rmdir(const char *path){
    return 0;
}
int vfs_link(const char *oldpath, const char *newpath){
    return 0;
}
int vfs_unlink(const char *path){
    return 0;
}
int vfs_open(const char *path, int flags){
    int id=vfs_access(path,flags);
    if(id==-1)
      return -1;
    return fd_open(id);
}
ssize_t vfs_read(int fd, void *buf, size_t nbyte){
    if(fd<0)
        return 0;
    int id=fildes[fd].inode;
    int len=0;
    if(vinode[id].filesystem==EXT2){
        len=ext2_read(vinode[id].fs,id,fildes[fd].offset,nbyte,buf);
        fildes[fd].offset+=len;
    }
    return len;
}
ssize_t vfs_write(int fd, void *buf, size_t nbyte){
    if(fd<0)
        return 0;
    int id=fildes[fd].inode;
    int len=0;
    if(vinode[id].filesystem==EXT2){
        len=ext2_write(vinode[id].fs,id,fildes[fd].offset,nbyte,buf);
        fildes[fd].offset+=len;
    }
    else if(vinode[id].filesystem==TTY){
        device_t *devtty=dev_lookup(vinode[id].name);
        len=tty_write(devtty,fildes[fd].offset,buf,nbyte);
        fildes[fd].offset+=len;
    }
    return len;
}
off_t vfs_lseek(int fd, off_t offset, int whence){
    switch (whence) 
    {
        case SEEK_SET: fildes[fd].offset = offset; break;
        case SEEK_CUR: fildes[fd].offset += offset; break;
        case SEEK_END: fildes[fd].offset = vinode[fildes[fd].inode].size + offset; break;
        default: return -1;
    }
    if (vinode[fildes[fd].inode].size < fildes[fd].offset)
        fildes[fd].offset = vinode[fildes[fd].inode].size;
    return fildes[fd].offset;
}
int vfs_close(int fd){
    fd_free(fd);
    return 1;
}

MODULE_DEF(vfs){
  .init = vfs_init,
  .access = vfs_access,
  .mount = vfs_mount,
  .unmount = vfs_unmount,
  .mkdir = vfs_mkdir,
  .rmdir = vfs_rmdir,
  .link = vfs_link,
  .unlink = vfs_unlink,
  .open = vfs_open,
  .read = vfs_read,
  .write = vfs_write,
  .lseek = vfs_lseek,
  .close = vfs_close,
};