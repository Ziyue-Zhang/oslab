#include <stdio.h>
#include <setjmp.h>
#include "co.h"
#define MAX_CO 100
typedef unsigned char uint8_t;
struct co {
    bool st;
    int num;
    jmp_buf buf;
    uint8_t stack[4096];
    void *__stack_backup;
};
struct co coroutines[MAX_CO];
struct co *current;
func_t cu_func;
func_t cu_arg;
int cunt;

void co_init() {
    cunt = 0;
}

struct co* co_start(const char *name, func_t func, void *arg) {
  coroutines[cunt].
  func(arg); // Test #2 hangs
  return NULL;
}

void co_yield() {
}

void co_wait(struct co *thd) {
}

