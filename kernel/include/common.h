#ifndef __COMMON_H__
#define __COMMON_H__

#include <kernel.h>
#include <nanos.h>

#define INT_MAX 2147483647
#define INT_MIN (-INT_MAX - 1)
#define FREE 0
#define RUNNING 1
#define SLEEP 2
#define DEAD 3
#define RUNNABLE 4
#define NOFILE 16

#define WR_ONLY 1
#define RD_ONLY 2
#define WRRD_ONLY 3

typedef struct file file_t;
struct task {
    char name[80];
    _Context context;
    int cpu;
    int id;
    int state;
    //struct task* next; //using in semaphere
    //long long align;    //align
    file_t *fildes[NOFILE];
    char stack[4096];
};
struct spinlock {
    volatile int locked;
    char name[80];      
    int cpu;
    //unsigned int pcs[10];
};
struct semaphore {
    volatile int value;
    char name[80];
    struct task *pool[32];
    struct spinlock lock;
    int l,r;
};
struct Mycpu {
    int ncli;
    int intena;
}mycpu[8];
struct task_st{
    int state;
    int cpu;
};
typedef struct sb_c_language{
    struct sb_c_language *next;
    int seq;
    int event;
    handler_t handler;
}handle;

typedef struct fsops fsops_t;

struct filesystem {
  fsops_t *ops;
  device_t *dev;
  void * myfs;
};

typedef struct inode inode_t;

struct fsops {
  void (*init)(struct filesystem *fs, const char *name, device_t *dev);
  //inode_t *(*lookup)(struct filesystem *fs, const char *path, int flags);
  int (*lookup)(struct filesystem *fs, const char *path, int flags);
  int (*close)(inode_t *inode);
};

typedef struct inodeops {
  int (*open)(file_t *file, int flags);
  int (*close)(file_t *file);
  ssize_t (*read)(file_t *file, char *buf, size_t size);
  ssize_t (*write)(file_t *file, const char *buf, size_t size);
  off_t (*lseek)(file_t *file, off_t offset, int whence);
  int (*mkdir)(const char *name);
  int (*rmdir)(const char *name);
  int (*link)(const char *name, inode_t *inode);
  int (*unlink)(const char *name);
} inodeops_t;

struct inode {
  int refcnt;
  void *ptr;       // private data
  filesystem_t *fs;
  inodeops_t *ops; // 在inode被创建时，由文件系统的实现赋值
                   // inode ops也是文件系统的一部分
};

struct file {
  int refcnt; // 引用计数
  int inode;
  uint64_t offset;
};
#endif