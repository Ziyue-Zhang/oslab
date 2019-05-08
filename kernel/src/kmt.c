#include <common.h>
#include <klib.h>
typedef struct task task_t;
typedef struct spinlock spinlock_t;
typedef struct semaphore sem_t;

 static void init(){

 }
 static int create(task_t *task, const char *name, void (*entry)(void *arg), void *arg){
	 return 0;
 }
 static void teardown(task_t *task){

 }
 static void spin_init(spinlock_t *lk, const char *name){

 }
 static void spin_lock(spinlock_t *lk){

 }
 static void spin_unlock(spinlock_t *lk){

 }
 static void sem_init(sem_t *sem, const char *name, int value){

 }
 static void sem_wait(sem_t *sem){

 }
 static void sem_signal(sem_t *sem){
	 
 }

typedef struct {
  void (*init)();
  int (*create)(task_t *task, const char *name, void (*entry)(void *arg), void *arg);
  void (*teardown)(task_t *task);
  void (*spin_init)(spinlock_t *lk, const char *name);
  void (*spin_lock)(spinlock_t *lk);
  void (*spin_unlock)(spinlock_t *lk);
  void (*sem_init)(sem_t *sem, const char *name, int value);
  void (*sem_wait)(sem_t *sem);
  void (*sem_signal)(sem_t *sem);
} MODULE(kmt);
