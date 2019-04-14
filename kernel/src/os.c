#include <common.h>
#include <klib.h>
#include <am.h>
void cli() {
	asm volatile ("cli");
}
void sti() {
	asm volatile ("sti");
}
void lock(intptr_t *lk) {
	cli();
	while (_atomic_xchg(lk, 1));
}
void unlock(intptr_t *lk) {
	_atomic_xchg(lk, 0);
	sti();
}

int temp=0;
intptr_t sb1,sb2;
static void os_init() {
  pmm->init();
}

static void hello() {
  lock(&sb1);    //we need to give a lock
  for (const char *ptr = "Hello from CPU #"; *ptr; ptr++) {
    _putc(*ptr);
  }
  _putc("12345678"[_cpu()]); _putc('\n');
  unlock(&sb1);
}

static void os_run() {
  hello();
  _intr_write(1);
  while (1) {
    //lock(&sb2);
    temp++;
    //printf("%d\n",temp);
    void * a = pmm->alloc(temp);
    pmm->free(a);
    pmm->free(a);
    assert(temp <5);
    //unlock(&sb2);
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
