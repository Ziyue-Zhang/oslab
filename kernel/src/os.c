#include <common.h>
#include <klib.h>
#include <lock.h>
int temp=0;
intptr_t sb;
static void os_init() {
  pmm->init();
}

static void hello() {
  for (const char *ptr = "Hello from CPU #"; *ptr; ptr++) {
    _putc(*ptr);
  }
  _putc("12345678"[_cpu()]); _putc('\n');
}

static void os_run() {
  hello();
  _intr_write(1);
  while (1) {
    lock(&sb);
    printf("%d\n",temp);
    temp++;
    void * a = kalloc(temp);
    kfree(a);
    if(temp>500)
      break;
    unlock(&sb);
    _yield();
  }
}

static _Context *os_trap(_Event ev, _Context *context) {
  return context;
}

static void os_on_irq(int seq, int event, handler_t handler) {
}

MODULE_DEF(os) {
  .init   = os_init,
  .run    = os_run,
  .trap   = os_trap,
  .on_irq = os_on_irq,
};
