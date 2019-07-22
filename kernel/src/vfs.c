#include <devices.h>
#include <common.h>
#include <klib.h>

#define VINODE_SIZE 1024

typedef struct Vinode{
  char name[80];
  char path[200];
  int me;
  int fa;
  int bro;
  int son;
  int use;
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

void vinode_free(int idx){
    vinode[idx].use=0;
}
void vinode_alloc(int idx){
    for(int i=0;i<VINODE_SIZE;i++){
        if(!vinode[i].use){
            vinode[i].use=1;
            return i;
        }
    }
    return -1;
}
void vfs_init(){
    memset(mount_table,0,sizeof(mount_table));
    memset(vinode,0,sizeof(vinode));
}
void vfs_lookup(char path){

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