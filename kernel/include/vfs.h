#ifndef __VFS_H__
#define __VFS_H__

#include <devices.h>
#include <common.h>
#include <klib.h>

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
int vinode_setroot();
int vinode_lookup(char *path);
int vinode_dot(int id);
int vinode_dotdot();
int vinode_dir();
int vinode_file();
void vinode_setdot(int id, int dot, int dotdot, int type, filesystem_t* fs);

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

#endif