#include <common.h>
#include <klib.h>
#include <am.h>
#include <x86.h>
typedef struct task task_t;
typedef struct spinlock spinlock_t;
typedef struct semaphore sem_t;

void panic(char *str){
  printf("%s\n", str);
  _halt(1);
}

void pushcli(void){
  int eflags = get_efl();
  cli();
  if(mycpu[_cup()].ncli == 0)
    mycpu[_cup()].intena = eflags & FL_IF;
  mycpu[_cup()].ncli += 1;
}

void popcli(void){
  if(get_efl()&FL_IF)
    panic("popcli - interruptible");
  if(mycpu[_cup()].ncli < 0)
    panic("popcli");
  if(mycpu[_cpu()].ncli == 0 && mycpu[_cup()].intena)
    sti();
}
int holding(struct spinlock *lock){
  int r;
  pushcli();
  r = lock->locked && lock->cpu == _cup();
  popcli();
  return r;
}

_Context *kmt_context_save (_Event ev, _Context *context){
  return NULL;
}
_Context *kmt_context_switch (_Event ev, _Context *context){
  return NULL;
}
 static void kmt_init();
 static int kmt_create(task_t *task, const char *name, void (*entry)(void *arg), void *arg);
 static void kmt_teardown(task_t *task);
 static void kmt_spin_init(spinlock_t *lk, const char *name);
 static void kmt_spin_lock(spinlock_t *lk);
 static void kmt_spin_unlock(spinlock_t *lk);
 static void kmt_sem_init(sem_t *sem, const char *name, int value);
 static void kmt_sem_wait(sem_t *sem);
 static void kmt_sem_signal(sem_t *sem);

 static void kmt_init(){
	os->on_irq(INT_MIN, _EVENT_NULL, kmt_context_save); 
  os->on_irq(INT_MAX, _EVENT_NULL, kmt_context_switch);
 }
 static int kmt_create(task_t *task, const char *name, void (*entry)(void *arg), void *arg){
	 return 0;
 }
 static void kmt_teardown(task_t *task){

 }
 static void kmt_spin_init(spinlock_t *lk, const char *name){
   lk = (spinlock_t *)pmm->alloc(sizeof(spinlock_t));
   lk->name = name;
   lk->locked = 0;
   lk->cpu = -1;    //this lock isn't on cpu 0,so we can't use 0
 }
 static void kmt_spin_lock(spinlock_t *lk){

 }
 static void kmt_spin_unlock(spinlock_t *lk){

 }
 static void kmt_sem_init(sem_t *sem, const char *name, int value){

 }
 static void kmt_sem_wait(sem_t *sem){

 }
 static void kmt_sem_signal(sem_t *sem){

 }

MODULE_DEF(kmt){
  .init = kmt_init,
  .create = kmt_create,
  .teardown = kmt_teardown,
  .spin_init = kmt_spin_init,
  .spin_lock = kmt_spin_lock,
  .spin_unlock = kmt_spin_unlock,
  .sem_init = kmt_sem_init,
  .sem_wait = kmt_sem_wait,
  .sem_signal = kmt_sem_signal,
};
