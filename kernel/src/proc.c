#include <proc.h>

void *proc_add(char *name){
    if(proc_num==PROC_SIZE){
        return NULL;
    }
    int id=proc_num;
    proc_num++;
    procfs[id].cpu=-1;
    procfs[id].mem=0;
    procfs[id].time=0;
    procfs[id].inode=id;
    strcpy(procfs[id].name,name);
    //printf("%s\n",name);
    return &procfs[id];
}
void proc_mem(uint32_t mem_size, int flag){
    if(flag==0){
        use_mem+=mem_size;
    }
    else if(flag==1){
        use_mem-=mem_size;
    }
}
void proc_time(proc_t *old, proc_t *new){
    if(old){
        old->cpu=-1;
    }
    if(new){
        new->cpu=_cpu();
        new->time++;
        jobs[new->cpu]=new->inode;
    }
}
int proc_build(int fa){
    for(int i=proc_num-2;i<proc_num;i++){
        int id=vinode_addfile(fa,FILE,procfs[i].name,PROC,NULL);
            //printf("%s\n",procfs[i].name);
        vinode[id].inode=i;
   }
    for(int i=0;i<proc_num-2;i++){
        int id=vinode_addfile(fa,FILE,procfs[i].name,PROC,NULL);
            //printf("%s\n",procfs[i].name);
        vinode[id].inode=i;
   }
    return 0;
}

int proc_init(filesystem_t* fs, const char* name, device_t *dev){
    proc_add("cpuinfo");
    proc_add("meminfo");    
    return 1;
}
int proc_dir(filesystem_t* fs, vinode_t *node, int num){
    if(num<=3){
        node->type=DIR;
        strcpy(node->name,procfs[num].name);
    }
    else{
        node->type=FILE;
        strcpy(node->name,procfs[num].name);
    }
    return 1;
}
ssize_t proc_read(int id, uint64_t offset, char *buf){
    int k=0;
    if(strcmp(procfs[id].name,"cpuinfo")==0){
        printf("\ncpuinfo:\n");
        for(int i=0;i<_ncpu();i++){
            int j=jobs[i];
            printf("pid:%d\n",j);
            printf("name:%s\n",procfs[j].name);
            printf("cpu_number:%d\n\n",procfs[j].cpu);
        }
    }
    else if(strcmp(procfs[id].name,"meminfo")==0){
        printf("\nmeminfo:\n");
        printf("using mem: %d b\n",use_mem);
        printf("free mem: %d b\n",int(tot_mem-use_mem));
        printf("tot mem: %d b\n",tot_mem);
    }
    else{
        printf("\ntaskinfo:\n");
        printf("pid:%d\n",id);
        printf("name:%d\n",procfs[id].name);
        printf("cpu_number:%d\n",procfs[id].cpu);
        printf("mem:%d\n",procfs[id].mem);
        printf("cpu_time:%d\n",procfs[id].time);
    }
    return k;
}