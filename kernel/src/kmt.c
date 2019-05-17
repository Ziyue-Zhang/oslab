#include <common.h>
#include <klib.h>
#include <am.h>
#include<x86.h>
#define LENGTH(arr) ((sizeof(arr) / sizeof(arr[0])))

extern _Context *kcontext();

typedef struct task task_t;
typedef struct spinlock spinlock_t;
typedef struct semaphore sem_t;

struct task *current_task[8];
struct task *tasks[28];
struct task_st tasks_st[28];
#define current (current_task[_cpu()])

 static void kmt_init();
 static int kmt_create(task_t *task, const char *name, void (*entry)(void *arg), void *arg);
 static void kmt_teardown(task_t *task);
 static void kmt_spin_init(spinlock_t *lk, const char *name);
 static void kmt_spin_lock(spinlock_t *lk);
 static void kmt_spin_unlock(spinlock_t *lk);
 static void kmt_sem_init(sem_t *sem, const char *name, int value);
 static void kmt_sem_wait(sem_t *sem);
 static void kmt_sem_signal(sem_t *sem);

spinlock_t LK,LK2;
int ncpu;
int task_cnt;
void panic(char *str){
  printf("%s\n", str);
  _halt(1);
}

void pushcli(void){
  int eflags = get_efl();
  cli();
  if(mycpu[_cpu()].ncli == 0)
    mycpu[_cpu()].intena = eflags & FL_IF;
  mycpu[_cpu()].ncli += 1;
}

void popcli(void){
  if(get_efl()&FL_IF)
    panic("popcli - interruptible");
  if(mycpu[_cpu()].ncli < 0)
    panic("popcli");
  if(mycpu[_cpu()].ncli == 0 && mycpu[_cpu()].intena)
    sti();
}
int holding(struct spinlock *lock){
  int r;
  pushcli();
  r = lock->locked && lock->cpu == _cpu();
  popcli();
  return r;
}

_Context *kmt_context_save (_Event ev, _Context *context){
  kmt_spin_lock(&LK);
  printf("nmsl\n");
  if (current) 
    current->context = *context;
  kmt_spin_unlock(&LK);
  return NULL;
}
_Context *kmt_context_switch (_Event ev, _Context *context){
  if(!task_cnt){
    return context;
  }
  kmt_spin_lock(&LK2);
  do {
    if (!current || current == tasks[LENGTH(tasks)-1]) {
      assert(tasks[0]);
      current = tasks[0];
    } else {
      current++;
    }
    //printf("nmsl\n");
    assert(current);
  } while (current->cpu != _cpu());
  //if(current!=NULL)
    //printf("%d\n",current->cpu);

  printf("[cpu-%d] Schedule: %s\n", _cpu(), current->name);
  kmt_spin_unlock(&LK2);
  return &current->context;
}

 static void kmt_init(){
   ncpu = _ncpu();
   task_cnt=0;
   printf("cpu num:%d\n",ncpu);
   kmt->spin_init(&LK, "lock");
   kmt->spin_init(&LK2, "lock2");
   for(int i = 0; i < 8;i++){
     mycpu[i].intena=1;   //interruptible
     mycpu[i].ncli=0;
     current_task[i]=NULL;
   }
   os->on_irq(INT_MIN, _EVENT_NULL, kmt_context_save); 
   os->on_irq(INT_MAX, _EVENT_NULL, kmt_context_switch);
    for(int i=0;i<LENGTH(tasks_st);i++){ //init tasks
      tasks_st[i].state=0;
      tasks_st[i].cpu=i%ncpu;
   }
 }
 static int kmt_create(task_t *task, const char *name, void (*entry)(void *arg), void *arg){
   ++task_cnt;
   strcpy(task->name, name);
   int i;
   for(i=0;i<LENGTH(tasks);i++){
     if(tasks_st[i].state==0){
       tasks_st[i].state=1;
       break;
     }
   }
   task->cpu=i%_ncpu();
   _Area stack = (_Area) { task->stack, task + 1 };
   task->context = *kcontext(stack, entry, (void *)arg);
   tasks[i]=task;
   //printf("%d\n",i);
   printf("create %s in cpu%d\n",tasks[i]->name,tasks[i]->cpu);
   return 0;
 }
 static void kmt_teardown(task_t *task){

 }
 static void kmt_spin_init(spinlock_t *lk, const char *name){
   strcpy(lk->name, name);
   lk->locked = 0;
   lk->cpu = -1;    //this lock isn't on cpu 0,so we can't use 0
 }
 static void kmt_spin_lock(spinlock_t *lk){
   printf("%s\n",lk->name);
  pushcli(); 
  if(holding(lk))
    panic("acquire");
  while(_atomic_xchg(&lk->locked, 1) != 0);
  __sync_synchronize();
  lk->cpu = _cpu();
 }
 static void kmt_spin_unlock(spinlock_t *lk){
  if(!holding(lk))
    panic("release");
  lk->cpu = -1;
  __sync_synchronize();
  asm volatile("movl $0, %0" : "+m" (lk->locked) : );
  popcli();
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
