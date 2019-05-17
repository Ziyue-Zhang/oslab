#ifndef __COMMON_H__
#define __COMMON_H__

#include <kernel.h>
#include <nanos.h>

#define INT_MAX 2147483647
#define INT_MIN (-INT_MAX - 1)
struct task {
    char name[80];
    _Context context;
    int cpu;
    long long align;    //align
    char stack[4096];
};
struct spinlock {
    int locked;
    char name[80];      
    int cpu;
    //unsigned int pcs[10];
};
struct semaphore {
    int value;
    char name[80];
    
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
#endif
