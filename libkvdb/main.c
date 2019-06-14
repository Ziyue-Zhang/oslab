#include "kvdb.h"
#include <stdio.h>
int main(){
    kvdb_t *db1 = malloc(sizeof(kvdb_t));
    kvdb_t *db2 = malloc(sizeof(kvdb_t));
    kvdb_open(db1, "a.db");
    kvdb_open(db2, "a.db");
    kvdb_put(db1,"sb","nmsll");
    kvdb_put(db2,"cnm","nmsl");
    const char *key1="sb";
    printf("%s\n",kvdb_get(db2,key1));
    //kvdb_get(db2,key1);
    return 0;
}
