/*#include "kvdb.h"
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
}*/
#include "kvdb.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdint.h>
#include <assert.h>


#include <sys/types.h>//for fork
#include <unistd.h>//for fork


#include <sys/wait.h>//for wait


#define NUMTHREADS 100
#define TESTNUM 100
#define FILESIZE 1000

#define handle_error(msg) \
    do {perror(msg); exit(EXIT_FAILURE); } while (0)

kvdb_t db;

/*
void *test1(void *data) {
    char *key = (char *) malloc(sizeof(char) * FILESIZE);
    char *value;
    kvdb_open(&db, "a.db"); // BUG: should check for errors
    char *buf = malloc(sizeof(char) * FILESIZE);
    if (buf == NULL) handle_error("malloc failure\n");
    for (int i = 0; i < TESTNUM; i++) {
        sprintf(buf, "operating-%d-systems", i);
        sprintf(key, "operating-%d-sys-hello", i);
        kvdb_put(&db, buf, key);
    }

    for (int i = 0; i < TESTNUM; i++) {
        sprintf(buf, "operating-%d-systems", i);
        sprintf(key, "operating-%d-sys-hello-world", i);
        if ((i % 2) == 0)
            kvdb_put(&db, buf, key);
        else {
            sprintf(key, "operating-%d-sys", i);
            kvdb_put(&db, buf, key);
        }
    }
    for (int i = 0; i < TESTNUM; i++) {
        sprintf(buf, "operating-%d-systems", i);
        value = kvdb_get(&db, buf);
        printf("[%s]: [%s]\n", buf, value);
        free(value);
    }
    free(key);
    free(buf);
    kvdb_close(&db);
}
*/

void *thread_test(void *data) {
    int open_ret;
    open_ret = kvdb_open(&db, "a.db");
    if (open_ret < 0) handle_error("open file failed in tests\n");

    char *key_buf = (char *) malloc(sizeof(char) * FILESIZE);
    char *value_buf = (char *) malloc(sizeof(char) * FILESIZE);
    char *value;
    uintptr_t thread_no = (uintptr_t) data;
    for (int i = 0; i < TESTNUM; i++) {
        //printf("first loop, %ld\n", (long int) data);
        sprintf(key_buf, "key-%d", i);
        sprintf(value_buf, "oldval-%d", i);
        int ret = kvdb_put(&db, key_buf, value_buf);
        assert(ret == 0);
    }
    for (int i = 0; i < TESTNUM; i++) {
        if ((i % 2) == 0) {
            //printf("second loop, %ld\n", (long int) data);
            sprintf(key_buf, "key-%d", i);
            sprintf(value_buf, "newval-%d", i);
            int ret = kvdb_put(&db, key_buf, value_buf);//偶数的被更新
            assert(ret == 0);
        }
    }

    for (int i = 0; i < TESTNUM; i++) {
        //printf("third loop, %ld\n", (long int) data);
        sprintf(key_buf, "key-%d", i);
        value = kvdb_get(&db, key_buf);
        assert(value != NULL);
        printf("[%s]: [%s]: [thread-%ld]\n", key_buf, value, (unsigned long) thread_no);
        //free(value);
    }
    free(key_buf);
    free(value_buf);
    return NULL;
}

int pthread_test() {
    int rc;
    uintptr_t t;
    pthread_t thread[NUMTHREADS];
    for (t = 0; t < NUMTHREADS; t++) {
        printf("Creating Thread %ld\n", (unsigned long) t + 1);
        rc = pthread_create(&thread[t], NULL, thread_test, (void *) (t));
        if (rc != 0) {
            printf("ERROR, return code is %d\n", rc);
            handle_error("ERROR\n");
        }
    }
    for (t = 0; t < NUMTHREADS; t++) {
        pthread_join(thread[t], NULL);
    }
    int err = kvdb_close(&db);
    if (err == -1) {
        handle_error("close file failed in tests\n");
    }
    printf("end of pthread_test\n");
    return 0;
}

int main() {
//    test1(100);


//    pid_t pid = fork();
//    if (pid == 0) {
//        pthread_test();
//    } else {
//        pid_t ppid = fork();
//        if (ppid == 0)
//            pthread_test();
//        else
//            pthread_test();
//    }
    pid_t pid = fork();
    if (pid == 0) {
        pthread_test();
        _exit(EXIT_SUCCESS);
    } else {
        pthread_test();
        wait(NULL);
        //Wait for child
        exit(EXIT_SUCCESS);
    }
    //perror("hello world");
    //pthread_test();

//    kvdb_open(&db, "a.db");
//    const char *key = "operating-systems";
//    kvdb_put(&db, key, "three-easy-pieces");
//    kvdb_put(&db, key, "three-easy-pieces-pk");
//    kvdb_put(&db, key, "three-easy");
//    kvdb_put(&db, "helloworld", "three-easy-pieces");
//    char *value = kvdb_get(&db, key);
//    printf("[%s]:[%s]\n", key, value);
//    free(value);
//    kvdb_close(&db);

    return 0;
}



//#include "kvdb.h"
//#include <stdlib.h>
//#include <stdio.h>
//#include <pthread.h>
//
//#include <stdint.h>
//#include <unistd.h>
//
//void *special_test(void *arg) {
//    void **args = arg;
//    kvdb_t *db = args[0];
//    int cnt = 0;
//    while (1) {
//        char key[5], val[20];
//        sprintf(key, "%d", cnt % 100);
//        sprintf(val, "%d", cnt);
//        kvdb_put(db, key, val);
//    }
//}
//
//void *test(void *arg) {
//    void **args = arg;
//    kvdb_t *db = args[0];
//    uintptr_t idx = 1000000 * (uintptr_t) args[1];
//    uintptr_t cnt = (uintptr_t) args[2];
//    while (1) {
//        char key[5], val[20];
//        sprintf(key, "%d", (int) cnt % 100);
//        sprintf(val, "%d", (int) (idx + cnt));
//        kvdb_put(db, key, val);
//        ++cnt;
//        if (cnt >= 1000000)cnt = 0;
//        usleep(1000);
//    }
//    return NULL;
//}
//
//#define THREADS 4
//#define ARGS 3
/*#define panic(fmt, ...) \
    fprintf(stderr, __FILE__ ":%d " fmt,__LINE__, ##__VA_ARGS__)
*/
//int main(int argc, char *argv[]) {
//    kvdb_t *db = malloc(sizeof(kvdb_t));
//    uintptr_t args[THREADS][ARGS];
//
//    if (db == NULL) {
//        panic("malloc failed. \n");
//        return 1;
//    }
//
//    if (kvdb_open(db, "yls.db")) {
//        panic("cannot open. \n");
//        return 1;
//    }
//
//    pthread_t pt[THREADS];
//    for (int i = 0; i < THREADS; i++) {
//        args[i][0] = (uintptr_t) db;
//        args[i][1] = i + 1;
//        args[i][2] = rand();
//        pthread_create(&pt[i], NULL, test, args[i]);
//    }
//
//    for (int i = 0; i < THREADS; i++) {
//        pthread_join(pt[i], NULL);
//    }
//
//    if (kvdb_close(db)) {
//        panic("cannot close. \n");
//        return 1;
//    }
//
//    return 0;
//}
