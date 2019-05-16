#ifndef __COMMON_H__
#define __COMMON_H__

#include <kernel.h>
#include <nanos.h>

#define INT_MAX 2147483647
#define INT_MIN (-INT_MAX - 1)
struct task {
    const char *name;
    _Context context;
    long long align;    //align
    char stack[4096];
};
struct task_st{
    int state;
    int cpu;
};
struct spinlock {
    int locked;
    const char *name;       //use const
    int cpu;
    //unsigned int pcs[10];
};
struct semaphore {};
struct Mycpu {
    int ncli;
    int intena;
}mycpu[8];
#endif
