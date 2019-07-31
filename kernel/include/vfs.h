#ifndef __VFS_H__
#define __VFS_H__

#include <devices.h>
#include <common.h>
#include <klib.h>
//#include <ext2.h>

#define VINODE_SIZE 1024
#define FILE_SIZE 128

#define EMPTY 0
#define FILE 1
#define DIR 2

#define PROC 0
#define VFS 1
#define EXT2 2
#define TTY 3

#define O_RDONLY 0
#define O_WRONLY 1
#define O_RDWR 2

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

typedef struct Vinode{
  char path[200];
  char name[80];
  int inode;
  int dotdot;
  int dot;
  int nxt;
  int son;
  int type;
  int mode;
  int size;
  int link
  int link_head;
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
vinode_t vinode[VINODE_SIZE];
file_t fildes[FILE_SIZE];

extern ssize_t tty_write(device_t *dev, off_t offset, const void *buf, size_t count);

void vinode_free(int idx);
int vinode_alloc(int type);
int vinode_setroot();
int vinode_find(char *path);
int vinode_lookup(char *path);
int vinode_dot(int id);
int vinode_dotdot();
int vinode_dir();
int vinode_file();
void vinode_setdot(int this_id, int dot, int dotdot, int fstype, filesystem_t* fs);
void vinode_setdotdot(int fa_id, int dot, int dotdot, int fstype, filesystem_t* fs);
void vinode_setdir(int id, int dot, int dotdot, char *name, int fstype, filesystem_t* fs);
void vinode_setfile(int id, int dot, int dotdot, char *name, int fstype, filesystem_t* fs);
int vinode_adddot();
int vinode_adddotdot();
int vinode_adddir(int fa, int type, char *name, int filesystem, filesystem_t *fs);
int vinode_addfile(int fa, int type, char *name, int filesystem, filesystem_t *fs);
int vinode_deldir(int fa, int this);
int vinode_delfile(int fa, int this);

int fd_alloc();
int fd_free(int fd);
int fd_open(int inode);

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

extern int ext2_read();
extern int ext2_write();

#endif