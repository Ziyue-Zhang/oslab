#include <proc.h>

int jobs[4];
proc_t procfs[PROC_SIZE];
int proc_num;
uint64_t use_mem;
uint64_t tot_mem;

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
void proc_mem(uint64_t mem_size, int flag){
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
        buf->type=DIR;
        strcpy(buf->name,procfs[num].name);
    }
    else{
        buf->type=FILE;
        strcpy(buf->name,procfs[num].name);
    }

}