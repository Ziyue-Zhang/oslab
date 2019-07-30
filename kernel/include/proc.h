#ifndef __PROC_H__
#define __PROC_H__

#include <vfs.h>
#include <common.h>
#include <klib.h>

#define PROC_SIZE 128

typedef struct proc{
    char name[20];
    int inode;
    int cpu;
    int time;
    int mem;
}proc_t;

#endif