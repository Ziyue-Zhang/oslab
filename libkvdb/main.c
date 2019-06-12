#include "kvdb.h"

int main(){
    kvdb_t *db1 = malloc(sizeof(kvdb_t));
    kvdb_t *db2 = malloc(sizeof(kvdb_t));
    kvdb_open(db1, "a.db");
    kvdb_open(db2, "a.db");
    kvdb_put(db1,"sb","nmsl");
    kvdb_put(db2,"cnm","nmsl");
    return 0;
}
