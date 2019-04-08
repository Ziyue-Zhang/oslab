#include <common.h>
#include <klib.h>
#include <lock.h>

#define MAXSIZE 1000
#define ALLOC_MIN 2<<7
#define FootLoc(p) (p+(p->size) - 1)

static uintptr_t pm_start, pm_end;
intptr_t alloc_lock;

/*typedef struct WORD{
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

void reclaimBoundTag(Space *pav, Space sp) {
	Space pre = (sp - 1)->uplink;
	Space next = sp + sp->size;
	int pTag = pre->tag;
	int nTag = next->tag;
	if(pTag == 1 && nTag == 1) {
		Space foot = FootLoc(sp);
		foot->tag = sp->tag = 0;
		if(pav == NULL) {
			*pav = sp->llink = sp->rlink = sp;
		}
		else {
			sp->rlink = *pav;
			sp->llink = (*pav)->llink;
			(*pre).llink = sp;
			sp->llink->rlink = sp;
			*pav = sp;
		}
	}
	else if(pTag == 0 && nTag == 1) {
		pre->size += sp->size;
		Space foot = FootLoc(pre);
		foot->tag = 0;
		foot->uplink = pre;
	}
	else if(pTag == 1 && nTag == 0) {
		sp->llink = next->llink;
		sp->rlink = next->rlink;
		next->llink->rlink = sp;
		next->rlink->llink = sp;
		sp->size += next->size;
		Space foot =FootLoc(sp);
		sp->tag = foot->tag = 0;
		foot->uplink = sp;
	}
	else {
		pre->rlink = next->rlink;
		pre->size += sp->size + next->size;
		Space foot = FootLoc(pre);
		foot->uplink = pre;
	}
	for(int i = 0;i < usCount; i++){
		if(sp == user[i]) {
			user[i] = NULL;
		}
	}
}*/
typedef struct node {
	bool flag;
	uint32_t size;
    struct node *next;
	void *start;
} mem;

static mem pool[1000];
static mem *head, *free;

static void pmm_init() {
  pm_start = (uintptr_t)_heap.start;
  pm_end   = (uintptr_t)_heap.end;
  printf("%d\n", pm_end-pm_start);
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
