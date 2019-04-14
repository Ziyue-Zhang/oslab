#include <common.h>
#include <klib.h>
//#include <lock.h>

#define MAXSIZE 10000

static uintptr_t pm_start, pm_end;
intptr_t alloc_lock;
extern void lock();
extern void unlock();

typedef struct node {
	uint32_t size;
  	struct node *next;
	struct node *pre;
	void *start;
} mem;

static mem pool[MAXSIZE];
static mem *head, *tail, *free;
int free_num, total;

void mem_init(){
	free_num = 1;
	total = MAXSIZE;
	pool[0].pre = NULL;
	pool[0].next = &pool[1];
	pool[0].size = (uintptr_t)_heap.end - (uintptr_t)_heap.start;
	//printf("%d\n",pool[0].size);
	pool[0].start = _heap.start;
	for(int i = 1; i < MAXSIZE; i++){
		pool[i].pre = &pool[i-1];
		if(i == MAXSIZE-1)
			pool[i].next=NULL;
		else
		  pool[i].next = &pool[i+1];
		pool[i].size = 0;
		pool[i].start = NULL;
	}
	head = NULL; 
	tail = &pool[MAXSIZE-1];
	assert(!tail->next);
	free = pool;
}

void * my_alloc(size_t size) {
	/*pm_start += size;
	return (void *)pm_start;*/
	if(size == 0){
		printf("Are you kidding?\n");
		return NULL;
	}
	if(!free_num || !total){
		printf("memory is full!\n");
		return NULL;
	}
	if(!tail || !free){
		assert(0);
	}
	int flag = 1;	
	if(tail->size != 0){	//all lists are used
		printf("total:%d free_num:%d\n",total,free_num);
		assert(total == free_num);
		flag = 0;
	}
	assert(free);
	mem *p=free;
	int i = 0;
	while(p && i < free_num){
		if(p->size>size)
			break;
		i++;
		p = p->next;
	}
	if(!p){
		printf("we don't have enough memory\n");
		assert(p);
		return NULL;
	}
	//printf("%d\n",total);
	void *ret=p->start;
	if(flag){
		if(p == free){
			mem *temp=tail;
			tail = tail->pre;
			tail->next = NULL;
      		temp->next=p->next;
			(p->next)->pre=temp;
			temp->pre=NULL;
			temp->size=p->size-size;
			temp->start=p->start+size;
			free=temp;
			p->next=head;
			if(head)
				head->pre=p;
			p->size=size;
			p->pre=NULL;
			head=p;
		}
		else {
			mem *temp=tail;
			tail = tail->pre;
			tail->next=NULL;
			temp->next=p->next;
			temp->pre=p->pre;
			p->next->pre=temp;
			p->pre->next=temp;
			temp->size=p->size-size;
			temp->start=p->start+size;
			p->pre=NULL;
			p->next=head;
			if(head)
				head->pre=p;
			p->size=size;
			head=p;
		}
	}
	else{
		if(p==free){
			free=free->next;
			free->pre=NULL;
			p->pre=NULL;
			p->next=head;
			if(head)
				head->pre=p;
			head=p;
		}
		else{
			p->pre->next=p->next;
			p->next->pre=p->pre;
			p->pre=NULL;
			p->next=head;
			if(head)
		  		head->pre=p;
			head=p;
		}
		--free_num;
	}
	--total;
	//printf("%d\n",free_num);
	return ret;
}

static void my_free(void *ptr) {
	mem * p = head;
	while(p){
		if(p->start == ptr)
			break;
		p=p->next;
	}
	if(!p){
		printf("Wrong address!");
		assert(p);
	}
	if(p == head){
		head=head->next;
		if(head)
			head->pre = NULL;
	}		
	else{
		p->pre->next=p->next;
		if(p->next)
			p->next->pre=p->pre;
	}
	if(!free) {
		p->next=free;
		p->pre=NULL;
		free=p;
		++total;
		++free_num;
	}
	else if(p->start<free->start){
		if(p->start+p->size != free->start){
			p->next=free;
			free->pre=p;
			p->pre=NULL;
			free=p;
			++total;
			++free_num;
				if(tail)
		assert(!tail->size);
		}
		else{
			free->start=p->start;
			free->size+=p->size;
			p->start=NULL;
			p->size=0;
			p->next=NULL;
			p->pre=tail;
			tail->next=p;
			tail=p;
			++total;
		}
	}
	else if(p->start>tail->start){
		if(tail->start+tail->size!=p->start){
			tail->next=p;
			p->pre=tail;
			p->next=NULL;
			tail=p;
			++total;
			++free_num;
		}
		else{
			tail->size+=p->size;
			tail->next=p;
			p->pre=tail;
			p->size=0;
			p->start=NULL;
			p->next=NULL;
			tail=p;
			++total;
		}	
	}
	else{
		mem*q=free->next;
		while(q){
			if(p->start<q->start)
				break;
			q=q->next;
		}
		if(q->pre->start+q->pre->size==p->start && p->start+p->size==q->start){
			q->pre->size+=p->size+q->size;
			q->pre->next=q->next;
			q->next->pre=q->pre;
			p->size=0;
			p->start=NULL;
			q->size=0;
			q->start=NULL;
			tail->next=p;
			p->pre=tail;
			q->next=NULL;
			q->pre=p;
			++total;
			free_num-=1;
			tail=q;
		}
		else if(q->pre->start+q->pre->size==p->start){
			q->pre->size+=p->size;
			p->size=0;
			p->start=NULL;
			p->next=NULL;
			p->pre=tail;
			tail->next=p;
			tail=p;
			++total;
		}
		else if(p->start+p->size==q->start){
			q->size+=p->size;
			q->start=p->start;
			p->size=0;
			p->start=NULL;
			p->next=NULL;
			p->pre=tail;
			tail->next=p;
			tail=p;
			++total;
		}
		else{
			q->pre->next=p;
			p->pre=q->pre;
			p->next=q;
			q->pre=p;
			++total;
			++free_num;
		}
	}
	if(tail)
		assert(!tail->size);
	//printf("%d\n",total);
	//printf("%d\n",free_num);
}

static void pmm_init() {
  pm_start = (uintptr_t)_heap.start;
  pm_end   = (uintptr_t)_heap.end;
//printf("%d\n", pm_end-pm_start);
  alloc_lock = 0;
  total = MAXSIZE;
  mem_init();
}

static void *kalloc(size_t size) {
  lock(&alloc_lock);
  void *ret = my_alloc(size);
  unlock(&alloc_lock);
  return ret;
  //return NULL;

}

static void kfree(void *ptr) {
	my_free(ptr);
}

MODULE_DEF(pmm) {
  .init = pmm_init,
  .alloc = kalloc,
  .free = kfree,
};
