#include <common.h>
#include <klib.h>
#include <lock.h>

#define MAXSIZE 1000
#define ALLOC_MIN 2<<7
#define FootLoc(p) (p+(p->size) - 1)

static uintptr_t pm_start, pm_end;
intptr_t alloc_lock;

typedef struct WORD{
	union {
		struct WORD *llink;
		struct WORD *uplink;
	};
	int tag;
	int size;
	struct WORD *rlink;
}*Space;

Space user[MAXSIZE] = {NULL};
int usCount = 0;

Space AllocBoundTag(Space *pav, int n) {
	Space p =  *pav;
	if(p == NULL) {
		printf("This memoty is null.\n");
		return NULL;
	}
	for(; p != NULL && p->size < n && p->rlink != *pav; p = p->rlink ) {
		if( p == NULL || p->size < n) {
			printf("wrong\n");
			return NULL;
		}
		*pav = p->rlink;
		if(p->size - n > ALLOC_MIN) {
			p->size -= n;
			Space foot = FootLoc(p);
			foot->uplink = p;
			foot->tag =0;
			p = foot + 1;
			p->size = n;
			foot = FootLoc(p);
			p->tag = 1;
			foot->tag = 1;
			foot->uplink = p;
		}
		else {
			if(p == *pav)
				*pav = NULL;
			else {
				Space foot = FootLoc(p);
				foot->tag = p->tag = 1;
				p->llink->rlink = p->rlink;
				p->rlink->llink = p->llink;
			}
		}
	}
	user[usCount++] = p;
	return p;
}



static void pmm_init() {
  pm_start = (uintptr_t)_heap.start;
  pm_end   = (uintptr_t)_heap.end;
  alloc_lock = 0;
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
