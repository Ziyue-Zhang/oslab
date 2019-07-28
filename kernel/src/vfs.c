#include <devices.h>
#include <common.h>
#include <klib.h>

#define VINODE_SIZE 1024
#define EMPTY 0
#define FILE 1
#define DIR 2
#define VFS 1
#define EXT2 2
#define TTY 3

typedef struct Vinode{
  char path[200];
  char name[80];
  int dotdot;
  int dot;
  int nxt;
  int son;
  int type;
  int link_inode;
  int link_count;
  int filesystem;       // vfs read/write/lseek must know it
  filesystem_t *fs;  
}vinode_t;
typedef struct fileroot{
  char name[80];
  filesystem_t *fs;
  int son;
  int use; 
}fileroot_t;
fileroot_t mount_table[16];
vinode_t vinode[1024];

void vinode_free(int idx);
int vinode_alloc(int type);
void vfs_init();
int vfs_lookup(char *path);
int vfs_access(const char *path, int mode);
int vfs_mount(const char *path, filesystem_t *fs);
int vfs_unmount(const char *path);
int vfs_mkdir(const char *path);
int vfs_rmdir(const char *path);
int vfs_link(const char *oldpath, const char *newpath);
int vfs_unlink(const char *path);
int vfs_open(const char *path, int flags);
ssize_t vfs_read(int fd, void *buf, size_t nbyte);
ssize_t vfs_write(int fd, void *buf, size_t nbyte);
off_t vfs_lseek(int fd, off_t offset, int whence);
int vfs_close(int fd);

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
int vinode_root(){
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

    vinode[id].dot=dot;
    vinode[id].dotdot=dotdot;
    vinode[id].nxt=-1;
    vinode[id].son=dot;
    vinode[id].link_inode=id;
    vinode[id].link_count=1;
    vinode[id].filesystem=VFS;
    vinode[id].fs=NULL;

    vinode[dot].dot=-1;
    vinode[dot].dotdot=dotdot;
    vinode[dot].nxt=dotdot;
    vinode[dot].son=id;
    vinode[dot].link_inode=dot;
    vinode[dot].link_count=1;
    vinode[dot].filesystem=VFS;
    vinode[dot].fs=NULL;

    vinode[dotdot].dot=dot;
    vinode[dotdot].dotdot=-1;
    vinode[dotdot].nxt=-1;
    vinode[dotdot].son=id;
    vinode[dotdot].link_inode=dotdot;
    vinode[dotdot].link_count=1;
    vinode[dotdot].filesystem=VFS;
    vinode[dotdot].fs=NULL;
    return id;
}
int vinode_lookup(char *path){
    return 0;
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