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

void file_unlock(int fd){
    struct flock lock;
    lock.l_type=F_UNLCK;
    lock.l_whence=SEEK_SET;
    lock.l_start=0;
    lock.l_len=0;
    lock.l_pid=getpid();
    fcntl(fd, F_SETLKW, &lock);
}

int kvdb_open(kvdb_t *db, const char *filename){
    if(open)
        return -1;
    pthread_mutex_lock(&(db->lock));
    db->open=1;
    db->num=0;
    FILE* fpp;
    fpp=fopen(filename, "a+");
    //file_lock(fileno(fpp));
    
    //file_unlock(fileno(fpp));
    fclose(fpp);
    db->fp=fpp;
    
    int len=strlen(filename);
    strncpy(db->filename,filename,len);
    pthread_mutex_unlock(&(db->lock));
    if(!fpp)
        return -1;
    else
        return 0;
}
int kvdb_close(kvdb_t *db){
    if(!db->open)
        return -1;
    pthread_mutex_lock(&(db->lock));
    db->open=0;
    pthread_mutex_unlock(&(db->lock));
}
int find_id(kvdb_t *db, const char *key){
    int tot=db->num;
    int i;
    for(i=0;i<tot;i++){
        if(strcmp(db->data[i].key,key)==0)
            break;
    }
    return i;
}
char temp_value[16 mb];
int kvdb_put(kvdb_t *db, const char *key, const char *value){
    /*pthread_mutex_lock(&(db->lock));
    FILE *fp;
    fp=fopen(db->filename, "a");
    //file_lock(fileno(fp));
    fwrite(db,sizeof(db),1,fp);
    //file_unlock(fileno(fp));
    fclose(fp);
    int id=find_id(db,key);
    if(id>db->num){
        db->num++;
        pthread_mutex_unlock(&(db->lock));
        return 1;
    }
    memset(temp_value,0,sizeof(temp_value));
    strcpy(temp_value,db->data[id].value);
    pthread_mutex_unlock(&(db->lock));*/

}

char *kvdb_get(kvdb_t *db, const char *key){
    pthread_mutex_lock(&(db->lock));
    FILE *fp;
    fp=fopen(db->filename, "r");
    //file_lock(fileno(fp));
    fwrite(db,sizeof(db),1,fp);
    //file_unlock(fileno(fp));
    fclose(fp);
    int id=find_id(db,key);
    if(id>db->num){
        pthread_mutex_unlock(&(db->lock));
        return NULL;
    }
    memset(temp_value,0,sizeof(temp_value));
    strcpy(temp_value,db->data[id].value);
    pthread_mutex_unlock(&(db->lock));
    return temp_value;
}
