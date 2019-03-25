#include <stdio.h>
#include <setjmp.h>
#include <stdbool.h>
#include "co.h"
#define MAX_CO 100
typedef unsigned char uint8_t;
uint8_t *__stack;
void *__stack_backup;

#if defined(__i386__)
  #define SP "%%esp"
#elif defined(__x86_64__)
  #define SP "%%rsp"
#endif

struct co {
    bool st;
    int num;
    jmp_buf buf;
    uint8_t stack[4096];
    void *stack_backup;
};
struct co coroutines[MAX_CO];
struct co *current;
func_t cu_func;
void *cu_arg;
int cunt;

void co_init() {
    cunt = 0;
}

struct co* co_start(const char *name, func_t func, void *arg) {
  coroutines[cunt].num = cunt;
  coroutines[cunt].st = true;
  cu_func = func;
  cu_arg = arg;
  __stack = coroutines[cunt].stack + sizeof(coroutines[cunt].stack);
  asm volatile("mov " SP ", %0; mov %1, " SP :
                 "=g"(__stack_backup) :
                 "g"(__stack));  
  coroutines[cunt].stack_backup = __stack_backup;
  
  func(arg); // Test #2 hangs
  
  __stack_backup = coroutines[cunt].stack_backup;
  asm volatile("mov %0," SP : : "g"(__stack_backup));
  ++cunt;
  return NULL;
}

void co_yield() {
}

void co_wait(struct co *thd) {
}

