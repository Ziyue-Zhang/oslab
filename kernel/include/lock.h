#include <am.h>

void cli() {
	asm volatile ("cli");
}
void sti() {
	asm volatile ("sti");
}
void lock(intptr_t *lk) {
	cli();
	while (atomic_xchg(lk, 1));
}
void unlock(intptr_t *lk) {
	atomic_xchg(lk, 0);
	sti();
}
	


