#include <common.h>
#include <klib.h>
#include <lock.h>

static uintptr_t pm_start, pm_end;

static void pmm_init() {
  pm_start = (uintptr_t)_heap.start;
  pm_end   = (uintptr_t)_heap.end;
}

static void *kalloc(size_t size) {
  spin_lock(&alloc_lock);
  pm_start += size;
  spin_unlock(&alloc_lock);
  return (void *)pm_start;
  //return NULL;
}

static void kfree(void *ptr) {
}

MODULE_DEF(pmm) {
  .init = pmm_init,
  .alloc = kalloc,
  .free = kfree,
};
