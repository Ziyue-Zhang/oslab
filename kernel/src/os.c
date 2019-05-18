#include <common.h>
#include <klib.h>
#include <am.h>
#include<devices.h>

handle * head;
handle pool[200];
int handle_cnt;

void lock(intptr_t *lk) {
	//cli();
	while (_atomic_xchg(lk, 1));
}
void unlock(intptr_t *lk) {
	_atomic_xchg(lk, 0);
	//sti();
}

int temp=0;
intptr_t sb1=0,sb2=0;
spinlock_t tp, alc;

/*sem_t empty, full, mutex;
int cunt;
const int maxk=6;
static void producer(void *arg){
  while(1){
    for(volatile int i = 0;i < 1000000; i++);
    kmt->sem_wait(&empty);
    kmt->sem_wait(&mutex);
    cunt++;
    if(cunt<0)
      printf("-%d ", -cunt);
    else
      printf("%d ", cunt);
    kmt->sem_signal(&mutex);
    kmt->sem_signal(&full);
  }
}
static void consumer(void *arg){
  while(1){
    for(volatile int i = 0;i < 1000000; i++);
    kmt->sem_wait(&full);
    kmt->sem_wait(&mutex);
    cunt--;
    if(cunt<0)
      printf("-%d ", -cunt);
    else
      printf("%d ", cunt);
    kmt->sem_signal(&mutex);
    kmt->sem_signal(&empty);
  }
}*/
extern ssize_t tty_write();
void echo_task(void *name){
  device_t *tty = dev_lookup(name);
  while(1){
    char line[128], text[128];
    sprintf(text, "(%s) $ ", name); tty_write(tty, 0, text, 5+strlen(name));
    int nread = tty->ops->read(tty, 0 ,line, sizeof(line));
    line[nread - 1] = '\0';
    sprintf(text, "Echo: %s.\n", line); tty_write(tty, 0, text, 8+strlen(line));
  }
}
void idle(void *arg){
  while(1){
    for (int volatile i = 0; i < 100000; i++);
    //_yield();
  }
}
/*void func(void *arg) {
  int cur = (intptr_t)arg;
  while (1) {

   printf("%d ", cur);
   printf("%d\n",_cpu());
   _yield();
    for (int volatile i = 0; i < 10000; i++);
  }
}

static void create_threads() {
  kmt->create(pmm->alloc(sizeof(task_t)),
              "test-thread-1", func, (void *)1);
  kmt->create(pmm->alloc(sizeof(task_t)),
              "test-thread-2", func, (void *)2);
  kmt->create(pmm->alloc(sizeof(task_t)),
              "test-thread-3", func, (void *)3);
  kmt->create(pmm->alloc(sizeof(task_t)),
              "test-thread-4", func, (void *)4); 
  kmt->sem_init(&empty, "buffer-empty", maxk);
  kmt->sem_init(&full, "buffer-full", 0);
  kmt->sem_init(&mutex, "mutex", 1);
  kmt->create(pmm->alloc(sizeof(task_t)),
              "test-thread-producer", producer, "xxx");
  kmt->create(pmm->alloc(sizeof(task_t)),
              "test-thread-consumer", consumer, "yyy");

  kmt->create(pmm->alloc(sizeof(task_t)),
              "test-thread-5", func, (void *)5);
  kmt->create(pmm->alloc(sizeof(task_t)),
              "test-thread-6", func, (void *)6);
  kmt->create(pmm->alloc(sizeof(task_t)),
              "test-thread-7", func, (void *)7);
  kmt->create(pmm->alloc(sizeof(task_t)),
              "test-thread-8", func, (void *)8);                 
  kmt->create(pmm->alloc(sizeof(task_t)),
              "test-thread-9", func, (void *)9);
  kmt->create(pmm->alloc(sizeof(task_t)),
              "test-thread-10", func, (void *)10);
  kmt->create(pmm->alloc(sizeof(task_t)),
              "test-thread-11", func, (void *)11);
  kmt->create(pmm->alloc(sizeof(task_t)),
              "test-thread-12", func, (void *)12); 
  kmt->create(pmm->alloc(sizeof(task_t)),
              "test-thread-13", func, (void *)13);
  kmt->create(pmm->alloc(sizeof(task_t)),
              "test-thread-14", func, (void *)14);
  kmt->create(pmm->alloc(sizeof(task_t)),
              "test-thread-15", func, (void *)15);
  kmt->create(pmm->alloc(sizeof(task_t)),
              "test-thread-16", func, (void *)16); 
  
}*/

static void os_init() {
  head=NULL;
  handle_cnt=0;
  pmm->init();
  kmt->init();
  kmt->spin_init(&alc, "alloc");
  kmt->spin_init(&tp, "ostrap");
  _vme_init(pmm->alloc, pmm->free);
  dev->init();
  kmt->create(pmm->alloc(sizeof(task_t)), "print", echo_task, "tty1");
  kmt->create(pmm->alloc(sizeof(task_t)), "print", echo_task, "tty2");
  kmt->create(pmm->alloc(sizeof(task_t)), "print", echo_task, "tty3");
  kmt->create(pmm->alloc(sizeof(task_t)), "print", echo_task, "tty4");
  kmt->create(pmm->alloc(sizeof(task_t)),
              "idle1", idle, (void *)1);
  kmt->create(pmm->alloc(sizeof(task_t)),
              "idle2", idle, (void *)2);
  kmt->create(pmm->alloc(sizeof(task_t)),
              "idle3", idle, (void *)3);
  kmt->create(pmm->alloc(sizeof(task_t)),
              "idle4", idle, (void *)4); 
  //create_threads();
}

static void hello() {
  //lock(&sb1);    //to print hello
  for (const char *ptr = "Hello from CPU #"; *ptr; ptr++) {
    _putc(*ptr);
  }
  _putc("12345678"[_cpu()]); _putc('\n');
  //unlock(&sb1);
}

static void os_run() {
  hello();
  _intr_write(1);
  while (1) {
    /*lock(&sb2);
    temp++;
    //printf("%d\n",temp);
    void*a = pmm->alloc(temp);
    pmm->free(a);
    //pmm->free(a);
    lock(&sb2);
    assert(temp <100000);
    unlock(&sb2);*/
    //printf("nnnnnnnnnnnmmmmmmmmmmmmmsssssssssssslllllllll\n");
    _yield();
    //printf("nnnnnnnnnnnmmmmmmmmmmmmmsssssssssssslllllllll\n");
  }
}

static _Context *os_trap(_Event ev, _Context *context) {
  kmt->spin_lock(&tp);
  //printf("%d\n",handle_cnt);
  _Context *ret = NULL;
  handle *handler = head;
  while(handler){
     //printf("inhandler\n");
    if (handler->event == _EVENT_NULL || handler->event == ev.event) {
      _Context *next = handler->handler(ev, context);
      //printf("finishhandle\n");
      if (next) 
        ret = next;
    }
    handler=handler->next;
  }
  //if(!head)
    //return context;
  kmt->spin_unlock(&tp);
  return ret;
}

static void os_on_irq(int seq, int event, handler_t handler) {
  if(!head){
    head=&pool[handle_cnt];
    handle_cnt+=1;
    head->next=NULL;
    head->seq=seq;
    head->event=event;
    head->handler=handler;
  }
  else{
    handle *p = head;
    handle *pre=p;
    while(p->next && p->seq<seq){
      pre=p;
      p=p->next;
    }
    if(p==head){
      if(!p->next){
        if(p->seq<seq){
          handle *temp=&pool[handle_cnt];
          handle_cnt+=1;
          temp->next=NULL;
          temp->seq=seq;
          temp->event=event;
          temp->handler=handler;
          p->next=temp;
        }
        else {
          handle *temp=&pool[handle_cnt];
          handle_cnt+=1;
          temp->next=head;
          temp->seq=seq;
          temp->event=event;
          temp->handler=handler;
          head=temp;
        }
      }
      else{
        handle *temp=&pool[handle_cnt];
        handle_cnt+=1;
        temp->next=head;
        temp->seq=seq;
        temp->event=event;
        temp->handler=handler;
        head=temp;
      }
    }

    else{
      if(!p->next){
        if(p->seq<seq){
          handle *temp=&pool[handle_cnt];
          handle_cnt+=1;
          temp->next=NULL;
          temp->seq=seq;
          temp->event=event;
          temp->handler=handler;
          p->next=temp;
        }
        else {
          handle *temp=&pool[handle_cnt];
          handle_cnt+=1;
          temp->next=p;
          temp->seq=seq;
          temp->event=event;
          temp->handler=handler;
          pre->next=temp;
        }
      }
      else{
          handle *temp=&pool[handle_cnt];
          handle_cnt+=1;
          temp->next=p;
          temp->seq=seq;
          temp->event=event;
          temp->handler=handler;
          pre->next=temp;
      }
    }
  }
}

MODULE_DEF(os) {
  .init   = os_init,
  .run    = os_run,
  .trap   = os_trap,
  .on_irq = os_on_irq,
};
