#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <stdbool.h>
#include <time.h>
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
}__attribute__((aligned(16)));
struct co coroutines[MAX_CO];
struct co *current;
func_t cu_func;
void *cu_arg;
int cunt;

void co_init() {
    cunt = 0;
    coroutines[cunt].st = true;
    coroutines[cunt].num = cunt;
    srand(time(NULL));
}

struct co* co_start(const char *name, func_t func, void *arg) {
  ++cunt;
  coroutines[cunt].num = cunt;
  coroutines[cunt].st = true;
  cu_func = func;
  cu_arg = arg;
  int val = setjmp(coroutines[0].buf);      //if we don't use it, will never return to this function.
  if(!val) {
    __stack = coroutines[cunt].stack + sizeof(coroutines[cunt].stack);
    asm volatile("mov " SP ", %0; mov %1, " SP :
                     "=g"(__stack_backup) :
                     "g"(__stack));  
    coroutines[cunt].stack_backup = __stack_backup;
    
    current = &coroutines[cunt];
    /*printf("%d\n",cunt);
    char * temp=(char *)cu_arg;
    printf("%s\n", temp);*/
    cu_func(cu_arg);
      
    current->st = 0;
    
    current = &coroutines[0];
    longjmp(current->buf, 1);
    
    __stack_backup = coroutines[cunt].stack_backup;
    asm volatile("mov %0," SP : : "g"(__stack_backup));
    
    
  }
  return &coroutines[cunt - 1];
}

void co_yield() {
 //printf("nmsl\n");
  int val = setjmp(current->buf);
  if (val == 0) {
    int next = rand() % cunt;
    while(next == current->num || !coroutines[next].st) {
        next = rand() % cunt;
    }
    current = &coroutines[next];
    longjmp(current->buf, 1);
  } else {
  //printf("nmsl\n");
    return;
  }
}

void co_wait(struct co *thd) {
printf("wait\n");
}

