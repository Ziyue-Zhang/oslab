#include "kvdb.h"

void file_lock(int fd){
    struct flock lock1;
    lock1.l_type=F_RDLCK;
    lock1.l_whence=SEEK_SET;
    lock1.l_start=0;
    lock1.l_len=0;
    lock1.l_pid=getpid();
    fcntl(fd, F_SETLKW, &lock1);
    struct flock lock2;
    lock2.l_type=F_WRLCK;
    lock2.l_whence=SEEK_SET;
    lock2.l_start=0;
    lock2.l_len=0;
    lock2.l_pid=getpid();
    fcntl(fd, F_SETLKW, &lock2);
}
int kvdb_open(kvdb_t *db, const char *filename){
    pthread_mutex_lock(&(db->lock));
    FILE* fpp;
    fpp=fopen(filename, "a+");
    db->fp=fpp;
    pthread_mutex_unlock(&(db->lock));
    return fpp?0:-1;
}
int kvdb_close(kvdb_t *db);
int kvdb_put(kvdb_t *db, const char *key, const char *value);
char *kvdb_get(kvdb_t *db, const char *key);
