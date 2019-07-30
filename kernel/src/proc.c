#include <proc.h>

int jobs[4];
proc_t procfs[PROC_SIZE];
int proc_num=0;

void *proc_add(char *name){
    if(proc_num==PROC_SIZR){
        return NULL;
    }
    int id=proc_num;
    proc_num++;
    procfs[id].cpu=-1;
    procfs[id].mem=0;
    procfs[id].inode=id-4;
    strcpy(procfs[id].name,name);
    return &procfs[id];
}