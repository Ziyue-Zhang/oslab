#include "kvdb.h"
pthread_mutex_t big_lock;
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
int kvdb_open(kvdb_t *db, const char *filename){
    /*if(db->open)        //already open
        return -1;*/
    pthread_mutex_lock(&big_lock);
    FILE* fp;
    fp=fopen(filename, "a+");
    if(!fp){
        pthread_mutex_unlock(&big_lock);
        return -1;
    }
    db->fp=fp;
    int len=strlen(filename);
    strncpy(db->filename,filename,len);
    db->open=1;
    fclose(fp);
    pthread_mutex_unlock(&big_lock);
    return 0;
}
int kvdb_close(kvdb_t *db){
    if(!db->open)       //altready close
        return -1;
    pthread_mutex_lock(&big_lock);
    FILE* fp;
    fp=fopen(db->filename, "a+");
    if(!fp){
        pthread_mutex_unlock(&big_lock);
        return -1;
    }
    if(fclose(fp)!=0){
        pthread_mutex_unlock(&big_lock);
        return -1;
    }
    db->open=0;
    pthread_mutex_unlock(&big_lock);
    return 0;
}
int kvdb_put(kvdb_t *db, const char *key, const char *value){
    if(!db->open)       //altready close
        return -1;
    pthread_mutex_lock(&big_lock);
    FILE* fp=fopen(db->filename,"a+");
    if(!fp){
        pthread_mutex_unlock(&big_lock);
        return -1;
    }
    int fd=fileno(fp);
    file_lock(fd);
    fseek(fp,0,SEEK_END);
    fwrite("\n",1,1,fp);
    fwrite(key,1,strlen(key),fp);
    fwrite("#",1,1,fp);
    fwrite("\n",1,1,fp);
    fwrite(value,1,strlen(value),fp);
    fwrite("#",1,1,fp);
    fwrite("\n",1,1,fp);
    fsync(fd);
    file_unlock(fd);
    if(fclose(fp)!=0){
        pthread_mutex_unlock(&big_lock);
        return -1;
    }
    pthread_mutex_unlock(&big_lock);
    return 0;

}
char *val=NULL;
char keyy[130];
char valuee[(1<<24)+2];
char *kvdb_get(kvdb_t *db, const char *key){
    if(!db->open) {      //altready close
        return NULL;
    }
    if(!val)
        free(val);
    pthread_mutex_lock(&big_lock);;
    FILE* fp=fopen(db->filename,"a+");
    if(!fp){
        pthread_mutex_unlock(&big_lock);
        return NULL;
    }
    int fd=fileno(fp);
    file_lock(fd);
    fseek(fp,0,SEEK_SET);
    /*while(1){
        if(!fgets(keyy,sizeof(keyy),fp))
            break;
        if(!fgets(valuee,sizeof(valuee),fp))
            break;
        int len=strlen(keyy);
        keyy[len-1]='\0';
        len=strlen(valuee);
        valuee[len-1]='\0';
        //printf("%s\n",keyy);
        //printf("%s\n",valuee);
        if(strcmp(keyy,key)==0){
            //printf("%s\n",valuee);
            free(val);
            val=strdup(valuee);
        }
    }*/
     while (!feof(fp)){
        printf("nmsl\n");
        fscanf(fp,"%s",keyy);
        printf("%s\n",keyy);
        int len=strlen(keyy);
        if(keyy[len-2]!='#'){
            printf("xjmsb\n");
            continue;
        }
        keyy[len-1]='\0';
        fscanf(fp,"%s",valuee);
        len=strlen(valuee);
        if(valuee[len-2]!='#')
            continue;
        valuee[len-1]='\0';
        //printf("%s\n",keyy);
        //printf("%s\n",valuee);
        if(strcmp(keyy,key)==0){
            //printf("%s\n",valuee);
            free(val);
            val=strdup(valuee);
        }
    }
    fsync(fd);
    file_unlock(fd);
    if(fclose(fp)!=0){
        pthread_mutex_unlock(&big_lock);
        return NULL;
    }
    pthread_mutex_unlock(&big_lock);
    //printf("%s\n",val);
    return val;
}
