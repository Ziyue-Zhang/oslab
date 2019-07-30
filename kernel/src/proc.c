#include <proc.h>

int jobs[4];
proc_t procfs[PROC_SIZE];
int proc_num;
uint32_t use_mem;
uint32_t tot_mem;

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
int proc_init(filesystem_t* fs, const char* name, device_t *dev){
    proc_num=0;
    use_mem=0;
    tot_mem=0;
    proc_add(".");
    proc_add("..");
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
    if(!offset)
        return 0;
    int k=0;
    if(id==2){
        k+=sprintf(buf+k,"\ncpuinfo:\n");
        for(int i=0;i<_ncpu();i++){
            int j=jobs[i].inode;
            k+=sprintf(buf+k,"pid:%d\n",j-4);
            k+=sprintf(buf+k,"name:%d\n",procfs[j].name);
            k+=sprintf(buf+k,"cpu_number:%d\n",procfs[j].cpu);
        }
    }
    else if(id==3){
        k+=sprintf(buf+k,"\nmeminfo:\n");
        k+=sprintf(buf+k,"using mem: %d kb\n",use_mem/1024);
        k+=sprintf(buf+k,"free mem: %d kb\n",(tot_mem-use_mem)/1024);
        k+=sprintf(buf+k,"tot mem: %d kb\n",tot_mem/1024);
    }
    else{
        k+=sprintf(buf+k,"\ntaskinfo:\n");
        k+=sprintf(buf+k,"pid:%d\n",j-4);
        k+=sprintf(buf+k,"name:%d\n",procfs[j].name);
        k+=sprintf(buf+k,"cpu_number:%d\n",procfs[j].cpu);
        k+=sprintf(buf+k,"mem:%d\n",procfs[j].mem);
        k+=sprintf(buf+k,"cpu_time:%d\n",procfs[j].time);
    }
    return k;
}