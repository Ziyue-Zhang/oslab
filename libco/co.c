#include <stdio.h>
#include <setjmp.h>
#include "co.h"
#define MAX_CO 100
typedef unsigned char   uint8_t;
struct co {
    char *name;
    int num;
    jmp_buf buf;
    uint8_t stack[4096];
    void *__stack_backup;
};
struct co coroutines[MAX_CO];

void co_init() {
}

struct co* co_start(const char *name, func_t func, void *arg) {
  func(arg); // Test #2 hangs
  return NULL;
}

void co_yield() {
}

void co_wait(struct co *thd) {
}

