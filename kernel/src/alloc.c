#include <common.h>
#include <klib.h>
#include <lock.h>
#include <unistd.h>

static uintptr_t pm_start, pm_end;
intptr_t alloc_lock;

static void pmm_init() {
  pm_start = (uintptr_t)_heap.start;
  pm_end   = (uintptr_t)_heap.end;
}

void * my_alloc(size_t size) {
	pm_start += size;
	return (void *)pm_start;
}

static void *kalloc(size_t size) {
  lock(&alloc_lock);
  void *ret = my_alloc(size);
  unlock(&alloc_lock);
  return ret;
  //return NULL;

}

static void kfree(void *ptr) {
}

MODULE_DEF(pmm) {
  .init = pmm_init,
  .alloc = kalloc,
  .free = kfree,
};
