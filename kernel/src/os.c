#include <common.h>
#include <klib.h>
#include <am.h>

handle * head;
handle pool[200];
int handle_cnt;
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
intptr_t sb1=0,sb2=0;



static void os_init() {
  head=NULL;
  handle_cnt=0;
  pmm->init();
  kmt->init();
  printf("create\n");
  _vme_init(pmm->alloc, pmm->free);
  //dev->init();
}

static void hello() {
  //lock(&sb1);    //to print hello
  for (const char *ptr = "Hello from CPU #"; *ptr; ptr++) {
    _putc(*ptr);
  }
  _putc("12345678"[_cpu()]); _putc('\n');
  //unlock(&sb1);
}

/*void echo_task(void *name){
  device_t *tty = dev_lookup(name);
  while(1){
    char line[128], text[128];
    sprintf(text, "(%s) $ ", name); tty_write(tty, 0, text, 5+strlen(name));
    int nread = tty->ops->read(tty, 0 ,line, sizeof(line));
    line[nread - 1] = '\0';
    sprintf(text, "Echo: %s.\n", line); tty_write(tty, 0, text, 8+strlen(line));
  }
}*/

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
    _yield();
  }
}

static _Context *os_trap(_Event ev, _Context *context) {
  printf("%d\n",handle_cnt);
  printf("nmsl\n");
  _Context *ret = NULL;
  handle *handler = head;
  while(handler){
     printf("sb1\n");
    if (handler->event == _EVENT_NULL || handler->event == ev.event) {
      _Context *next = handler->handler(ev, context);
      printf("sb2\n");
      if (next) 
        ret = next;
    }
    handler=handler->next;
  }
  //if(!head)
    //return context;
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
