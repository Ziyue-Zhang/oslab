#include "kvdb.h"


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
