#ifndef __PROC_H__
#define __PROC_H__

#include <vfs.h>
#include <common.h>
#include <klib.h>

#define PROC_SIZE 128

int jobs[4];
proc_t procfs[PROC_SIZE];
int proc_num;
uint64_t use_mem;
uint64_t tot_mem;

#endif