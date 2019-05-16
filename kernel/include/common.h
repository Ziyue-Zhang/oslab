#ifndef __COMMON_H__
#define __COMMON_H__

#include <kernel.h>
#include <nanos.h>

#define INT_MAX 2147483647
#define INT_MIN (-INT_MAX - 1)
struct task {
    const char *name;
    _Context context;
    int cpu;
    long long align;    //align
    char stack[4096];
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
struct task_st{
    int state;
    int cpu;
};
typedef struct sb_c_language{
    struct handle *next;
    int seq;
    int event;
    handler_t handler;
}handle;
#endif
