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
    vinode[id].link=0;
    vinode[id].link_head=id;
    vinode[id].link_inode=id;
    vinode[id].link_count=1;
    vinode[id].filesystem=VFS;
    vinode[id].fs=NULL;

    vinode[dot].dot=dot;
    vinode[dot].dotdot=dotdot;
    vinode[dot].nxt=dotdot;
    vinode[dot].son=id;
    vinode[dot].link=0;
    vinode[dot].link_head=dot;
    vinode[dot].link_inode=dot;
    vinode[dot].link_count=1;
    vinode[dot].filesystem=VFS;
    vinode[dot].fs=NULL;

    vinode[dotdot].dot=dot;
    vinode[dotdot].dotdot=dotdot;
    vinode[dotdot].nxt=-1;
    vinode[dotdot].son=id;
    vinode[dotdot].link=0;
    vinode[dotdot].link_head=dotdot;
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
    vinode[dot].link=0;
    vinode[dot].link_head=dot;
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
    vinode[dotdot].link=0;
    vinode[dotdot].link_head=dotdot;
    vinode[dotdot].link_inode=dotdot;
    vinode[dotdot].link_count=1;
    vinode[dotdot].filesystem=fstype;
    vinode[dotdot].fs=fs;
}
void vinode_setdir(int id, int dot, int dotdot, char *name, int fstype, filesystem_t* fs){
    strcpy(vinode[id].name,name);
    strcpy(vinode[id].path,vinode[dot].path);
    //printf("%d\n",dot);
    //printf("%s\n",vinode[dot].path);
    //int len=strlen(vinode[id].path);
    //vinode[id].path[len-1]='\0';
    strcat(vinode[id].path,name);
    strcat(vinode[id].path,"/");
    vinode[id].dot=dot;
    vinode[id].dotdot=dotdot;
    vinode[id].nxt=-1;
    vinode[id].son=-1;
    vinode[id].link=0;
    vinode[id].link_head=id;
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
    vinode[id].link=0;
    vinode[id].link_head=id;
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
            if(vinode[i].type==DIR && vinode[i].dot!=i && vinode[i].dotdot!=i){
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
            if(vinode[i].type==DIR && vinode[i].dot!=i && vinode[i].dotdot!=i){
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
        while(vinode[id].link_inode!=id){
            id=vinode[id].link_inode;
        }
    }
    return id;
}
int vinode_findabs(char *path){
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
            while(vinode[son_id].link_inode!=son_id){
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
    char name[80];
    strcpy(temp, path);
    if(vinode_find(temp)!=-1)
      return -1;
    int i=strlen(temp)-1;
    while(1){
        if(temp[i]=='/')
            break;
        i--;
    }
    strcpy(name,temp+i+1);
    temp[i+1]='\0';
    int fa=vinode_find(temp);
    //printf("%s\n",vinode[fa].path);
    int id=vinode_adddir(fa,DIR,name,VFS,NULL);
    //printf("%s\n",vinode[id].path);
    return id;
}
int vfs_rmdir(const char *path){
    char temp[200];
    strcpy(temp,path);
    //printf("%s\n",path);
    int this=vinode_find(temp);
    if(this==-1){
        return 1;
    }
    if(vinode[this].type!=DIR){
        return 2;
    }
    if(vinode[this].link_head==this){
        temp[0]='\0';
        strcpy(temp,vinode[this].path);
        int i=strlen(temp)-1;
        if(temp[i]=='/')
           temp[i]='\0';
        i=strlen(temp)-1;
        while(1){
           if(temp[i]=='/')
               break;
            i--;
        }
        temp[i]='\0';
        int fa=vinode_find(temp);
    //printf("%d %d\n",fa,this);
        vinode_deldir(fa, this);
    }
    else{
        this=vinode[this].link_head;
        printf("%s\n",vinode[this].name);
        while(1){
            if(vinode[this].type!=FREE){
                if(vinode[this].type==DIR){
                    temp[0]='\0';
                    strcpy(temp,vinode[this].path);
                    int i=strlen(temp)-1;
                    if(temp[i]=='/')
                    temp[i]='\0';
                    i=strlen(temp)-1;
                    while(1){
                    if(temp[i]=='/')
                        break;
                    i--;
                    }
                    temp[i]='\0';
                    int fa=vinode_find(temp);
                    vinode_deldir(fa, this);
                }
                else if(vinode[this].type==FILE){
                    temp[0]='\0';
                    strcpy(temp,vinode[this].path);
                    int i=strlen(temp)-1;
                    if(temp[i]=='/')
                    temp[i]='\0';
                    i=strlen(temp)-1;
                    while(1){
                    if(temp[i]=='/')
                        break;
                    i--;
                    }
                    temp[i]='\0';
                    int fa=vinode_find(temp);
                    vinode_delfile(fa, this);
                }
            }
            if(vinode[this].link_inode==this)
                break;
            int q=this;
            this=vinode[this].link_inode;
            vinode[q].link=0;
            vinode[q].link_inode=q;
        }
    }
    return 0;
}
int vfs_link(const char *oldpath, const char *newpath){
    char temp[200];
    temp[0]='\0';
    strcpy(temp,oldpath);
    int old=vinode_find(temp);
    if(old==-1){
        return -2;
    }
    temp[0]='\0';
    strcpy(temp,newpath);
    int new=vinode_find(temp);
    if(new==-1){
        return -1;
    }
    vinode[old].link=1;
    vinode[old].link_inode=vinode[new].link_head;
    int id=old;
    while(1){
        vinode[id].link_head=old;
        if(vinode[id].link_inode==id)
            break;
        id=vinode[id].link_inode;
    }
    return 0;
}
int vfs_unlink(const char *path){
    char temp[200];
    temp[0]='\0';
    strcpy(temp,path);
    int id=vinode_find(temp);
    if(id==-1){
        return -1;
    }
    if(vinode[id].link_inode==id){
        return 0;
    }
    vinode[id].link=0;
    int head=vinode[id].link_head;
    if(head==id){
        head=vinode[id].link_inode;
        int p=head;
        while(1){
            vinode[p].link_head=head;
            if(vinode[p].link_inode==p)
                break;
        }
    }
    else{
        int p=head;
        while(1){
            if(vinode[p].link_inode==id){
                break;
            }
            vinode[p].link_inode=vinode[id].link_inode;
        }
    }
    vinode[id].link_head=id;
    vinode[id].link_inode=id;
    return 1;
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