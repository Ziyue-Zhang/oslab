#include "kvdb.h"

void file_lock(int fd){
    struct flock lock;
    lock.l_type=F_WRLCK;
    lock.l_whence=SEEK_SET;
    lock.l_start=0;
    lock.l_len=0;
    lock.l_pid=getpid();
    fcntl(fd, F_SETLKW, &lock);
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

/*int kvdb_open(kvdb_t *db, const char *filename){
    if(db->open)
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
    return 0;
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
    pthread_mutex_lock(&(db->lock));
    FILE *fp;
    fp=fopen(db->filename, "r");
    //file_lock(fileno(fp));
    fwrite(db,sizeof(db),1,fp);
    //file_unlock(fileno(fp));
    fclose(fp);
    int id=find_id(db,key);
    if(id>db->num){
        db->num++;
    }
    fp=fopen(db->filename, "rb+");
    memset(temp_value,0,sizeof(temp_value));
    strcpy(temp_value,value);
    fseek(fp,sizeof(struct block)*id+128, SEEK_SET);
    fwrite(temp_value,sizeof(temp_value),1,fp);
    pthread_mutex_unlock(&(db->lock));
    return 0;
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
}*/

char *val[16 mb];
int kvdb_open(kvdb_t *db, const char *filename){
    if(db->open)        //already open
        return -1;
    pthread_mutex_lock(&(db->lock));
    db->open=1;
    FILE* fp;
    fp=fopen(filename, "a+");
    db->fp=fp;
    int len=strlen(filename);
    strncpy(db->filename,filename,len);
    pthread_mutex_unlock(&(db->lock));
    if(!fp)
        return -1;
    else
        return 0;
}
int kvdb_close(kvdb_t *db){
    if(!db->open)
        return -1;
    pthread_mutex_lock(&(db->lock));
    db->open=0;
    FILE* fp=db->fp;
    int fd=fileno(fp);
    if(!fclose(fp)){
        pthread_mutex_unlock(&(db->lock));
        return -1;
    }
    pthread_mutex_unlock(&(db->lock));
    return 0;
}
int kvdb_put(kvdb_t *db, const char *key, const char *value){
    return 0;
}
char *kvdb_get(kvdb_t *db, const char *key){
    return NULL;
}
