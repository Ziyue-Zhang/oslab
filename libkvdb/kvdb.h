#ifndef __KVDB_H__
#define __KVDB_H__

#include<pthread.h>
#include<stdio.h>
#include<sys/file.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<assert.h>

#define mb *1024*1024

/*struct block{
    char key[128];
    char value[16 mb];
};*/

struct kvdb {
    //struct block data[64];
    char filename[200];
    pthread_mutex_t lock;
    int open;
    FILE *fp;
    //int num;
};
typedef struct kvdb kvdb_t;

int kvdb_open(kvdb_t *db, const char *filename);
int kvdb_close(kvdb_t *db);
int kvdb_put(kvdb_t *db, const char *key, const char *value);
char *kvdb_get(kvdb_t *db, const char *key);

#endif
