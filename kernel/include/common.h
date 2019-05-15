#ifndef __COMMON_H__
#define __COMMON_H__

#include <kernel.h>
#include <nanos.h>

#define INT_MAX 2147483647
#define INT_MIN (-INT_MAX - 1)
struct task {
    const char *name;
    _Context context;
    char stack[4096];
};
struct spinlock {
    unsigned int locked;
    char *name;
    int cpu;
    unsigned int pcs[10];
};
struct semaphore {};
struct Mycpu {
    int ncli;
    int intena;
}mycpu[8];
#endif
