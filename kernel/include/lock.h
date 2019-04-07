#include <am.h>

void cli() {
	asm volatile ("cli");
}
void sti() {
	asm volatile ("sti");
}
void lock(lock_t *m) {
	cli();
	while (atomic_xchg(&lk->locked, 1));
}
void unlock(lock_t *lk) {
	atomic_xchg(&lk->locked, 0);
	sti();
}
	


