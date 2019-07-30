#include<vfs.h>

void vinode_free(int idx){
    vinode[idx].type=0;
}
int vinode_alloc(int type){
    for(int i=0;i<VINODE_SIZE;i++){
        if(!vinode[i].type){
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
    strcpy(vinode[dot].path,"/.");
    strcpy(vinode[dotdot].name,"..");
    strcpy(vinode[dotdot].path,"/..");

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
void vinode_setdot(int this_id, int dot, int dotdot, int type, filesystem_t* fs){
    strcpy(vinode[dot].name,".");
    strcpy(vinode[dot].path,vinode[this_id].path);
    strcat(vinode[dot].path,".");
    vinode[dot].dot=dot;
    vinode[dot].dotdot=dotdot;
    vinode[dot].nxt=dotdot;
    vinode[dot].son=this_id;
    vinode[dot].link_inode=dot;
    vinode[dot].link_count=1;
    vinode[dot].filesystem=type;
    vinode[dot].fs=fs;
}
void vinode_setdotdot(int fa_id, int dot, int dotdot, int type, filesystem_t* fs){
    strcpy(vinode[dotdot].name,"..");
    strcpy(vinode[dotdot].path,vinode[fa_id].path);
    strcat(vinode[dotdot].path,"..");
    vinode[dotdot].dot=dot;
    vinode[dotdot].dotdot=dotdot;
    vinode[dotdot].nxt=-1;
    vinode[dotdot].son=fa_id;
    vinode[dotdot].link_inode=dotdot;
    vinode[dotdot].link_count=1;
    vinode[dotdot].filesystem=type;
    vinode[dotdot].fs=fs;
}
void vinode_setdir(int id, int dot, int dotdot, char *name, int type, filesystem_t* fs){
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
    vinode[id].filesystem=type;
    vinode[id].fs=fs;  
}
void vinode_setfile(int id, int dot, int dotdot, char *name, int type, filesystem_t* fs){
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
    vinode[id].filesystem=type;
    vinode[id].fs=fs;  
}

int vinode_dot(int id){
    int dot=vinode_alloc(DIR);
    return dot;
}
int vinode_dotdot(){
    int dotdot=vinode_alloc(DIR);

    return dotdot;
}
int vinode_dir(){
    return 0;
}
int vinode_file(){
    return 0;
}
int vinode_find(char *path){
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
        else
            id=son_id;        
        //printf("%s\n",name);
    }
    return id;
}
void vfs_init(){
    memset(mount_table,0,sizeof(mount_table));
   
}

int vfs_lookup(char *path){
    return 0;
}
int vfs_access(const char *path, int mode){
  return 0;
}
int vfs_mount(const char *path, filesystem_t *fs){
  return 0;
}
int vfs_unmount(const char *path){
  return 0;
}
int vfs_mkdir(const char *path){
  return 0;
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
  return 0;
}
ssize_t vfs_read(int fd, void *buf, size_t nbyte){
  return 0;
}
ssize_t vfs_write(int fd, void *buf, size_t nbyte){
	/*ssize_t fs_size = fs_filesz(fd);
	ssize_t fs_offset = file_table[fd].open_offset;	
	size_t n = len;
   	if(fs_offset + len > fs_size)
		n = fs_size - fs_offset;
	len = fd < 3 ? len : n;
	if(fd >= 3)
		file_table[fd].open_offset += len;		
	if(file_table[fd].write)
		return file_table[fd].write(buf, fs_offset, len);
	else 
	{						
		ext2_write(buf, fs_offset + file_table[fd].disk_offset, len);
		return len;
	}*/
    return 0;
}
off_t vfs_lseek(int fd, off_t offset, int whence){
  return 0;
}
int vfs_close(int fd){
  return 0;
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