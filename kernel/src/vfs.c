#include <devices.h>
#include <common.h>
#include <klib.h>
  void vfs_init(){

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
      return 0
  }
  int vfs_open(const char *path, int flags){
      return 0
  }
  ssize_t vfs_read(int fd, void *buf, size_t nbyte){
      return 0;
  }
  ssize_t vfs_write(int fd, void *buf, size_t nbyte){
      return 0;
  }
  off_t vfs_lseek(int fd, off_t offset, int whence){
      return 0;
  }
  int vfs_close(int fd){
      return 0;
  }