#include <common.h>
#include <klib.h>
typedef struct task task_t;
typedef struct spinlock spinlock_t;
typedef struct semaphore sem_t;

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
	 
 }
 static int kmt_create(task_t *task, const char *name, void (*entry)(void *arg), void *arg){
	 return 0;
 }
 static void kmt_teardown(task_t *task){

 }
 static void kmt_spin_init(spinlock_t *lk, const char *name){

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
