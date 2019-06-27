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
struct task *tasks[32];
struct task_st tasks_st[32];
int tkfree[32];
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
spinlock_t tp,alc;
int ncpu;
int task_cnt;
void panic(char *str){
  printf("%s\n", str);
  _halt(1);
}
/*intptr_t atmlk=0;
void atm_lock(intptr_t *lk){
  while(_atomic_xchg(lk,1))
    for(int volatile i=0;i<100;i++);
}
void atm_unlock(intptr_t *lk){
  _atomic_xchg(lk,0);
}*/
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
  mycpu[_cpu()].ncli-=1;
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
  //kmt_spin_lock(&LK);
  //printf("nmsl\n");
  if (current) {
    /*if(current->state==SLEEP){
      printf("%s\n",current->name);
      assert(0);
    }*/
    //assert(current->state==RUNNING);
    current->context = *context;
  }
  //kmt_spin_unlock(&LK);
  return NULL;
}
_Context *kmt_context_switch (_Event ev, _Context *context){
  /*if(!task_cnt){
    return context;
  }*/
  //kmt_spin_lock(&LK);
  /*do {
    if (!current || current == tasks[LENGTH(tasks)-1]) {
      assert(tasks[0]);
      current = tasks[0];
    } else {
      int i = current->id+1;
      current=tasks[i];
      //if(current)
      //printf("current:%s\n",current->name);
    }
    //printf("nmsl\n");
    //assert(current);
    //printf("%d %d\n", current->cpu, _cpu());
    //if(tasks[0])
    //if(current->state>=SLEEP)
    //printf("sleep%s\n",current->name);
    //printf("%s\n",tasks[0]->name);
  } while (!current || current->cpu != _cpu() || current->state==SLEEP);*/
  //if(current!=NULL)
    //printf("%d\n",current->cpu);
  //if(current->id%2!=_cpu())
    //assert(0);
  //printf("id:%d [cpu-%d] Schedule: %s\n", current->id, _cpu(), current->name);
   //printf("%s\n",current->name);
  if(current && current->state==RUNNING){
    current->state=RUNNABLE;
  }
  int id=0;
  if(current) id=current->id;
  while(1){
    id=(id+1)%32;
    if(!tasks[id])
      continue;
    //if(tasks[id]->state==RUNNABLE && tasks[id]->cpu==_cpu())
    if(tasks[id]->state==RUNNABLE)
      break;
  }
  current=tasks[id];
  current->state=RUNNING;
  //kmt_spin_unlock(&LK);
  return &current->context;
}

 static void kmt_init(){
   ncpu = _ncpu();
   task_cnt=0;
   printf("cpu num:%d\n",ncpu);
   kmt->spin_init(&LK, "lock_task");
   kmt->spin_init(&LK2, "lock_sem");
   kmt->spin_init(&alc, "alloc");
   kmt->spin_init(&tp, "ostrap");
   for(int i = 0; i < 8;i++){
     mycpu[i].intena=0;   //interruptible
     mycpu[i].ncli=0;
     current_task[i]=NULL;
   }
   os->on_irq(INT_MIN, _EVENT_NULL, kmt_context_save); 
   os->on_irq(INT_MAX, _EVENT_NULL, kmt_context_switch);
    for(int i=0;i<LENGTH(tasks_st);i++){ //init tasks
      tkfree[i]=FREE;
   }
 }
 static int kmt_create(task_t *task, const char *name, void (*entry)(void *arg), void *arg){
   kmt_spin_lock(&LK);
   strcpy(task->name, name);
   task->state=RUNNABLE;
   _Area stack = (_Area) { task->stack, task + 1 };
   task->context = *kcontext(stack, entry, (void *)arg);
   int i;
   for(i=0;i<task_cnt;i++){
     if(tasks[i]==NULL){
       break;
     }
   }
   if(i==task_cnt)
    ++task_cnt;
   if(task_cnt>32)
    assert(0);
   task->id=i;
   task->cpu=i%_ncpu();
   tasks[i]=task;
   //printf("%d\n",i);
   //printf("create %s in cpu%d\n",tasks[i]->name,tasks[i]->cpu);
   kmt_spin_unlock(&LK);
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
   //printf("%s in cpu%d\n",lk->name,_cpu());
  pushcli(); 
  if(holding(lk))
    panic("acquire");
  while(_atomic_xchg(&lk->locked, 1) != 0);
  //printf("%s\n",lk->name);
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
  //printf("unlocked:%s\n",lk->name);
 }
  static void kmt_sem_init(sem_t *sem, const char *name, int value){
   sem->value=value;
   strcpy(sem->name,name);
   sem->l=0;
   sem->r=0;
   kmt_spin_init(&sem->lock,name);
 }
 static void block(sem_t *sem){
   //kmt_spin_lock(&LK2);
   current->state=SLEEP;
   sem->pool[sem->r]=current;
   sem->r=sem->r+1;
   if(sem->r==32)
    sem->r=0;
   //kmt_spin_unlock(&LK2);
   kmt_spin_unlock(&sem->lock);
   _yield();
   //kmt_spin_lock(&sem->lock);
 }
 static void wakeup(sem_t *sem){
   if(sem->r-sem->l==0){
     panic("wake");
   }
   //kmt_spin_lock(&LK2);
   task_t *temp=sem->pool[sem->l];
   sem->l=sem->l+1;
   if(sem->l==32)
    sem->l=0;
   temp->state=RUNNABLE;
   kmt_spin_unlock(&sem->lock);
   //kmt_spin_unlock(&LK2);  
 }
 static void kmt_sem_wait(sem_t *sem){
   //printf("wait %s\n",sem->name);
   kmt_spin_lock(&sem->lock);
   sem->value--;
   if(sem->value<0)
     block(sem);
   kmt_spin_unlock(&sem->lock);
 }
 static void kmt_sem_signal(sem_t *sem){
   kmt_spin_lock(&sem->lock);
   sem->value++;
   if(sem->value<=0)
     wakeup(sem);
   kmt_spin_unlock(&sem->lock);
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
