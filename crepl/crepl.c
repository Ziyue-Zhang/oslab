#include <dlfcn.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#define N 10000
int cnt,num;
char str[N];
char expr[N];
char name[N];
struct Node{
  char name[80];
}fun[10000];
int main(int argc, char *argv[]) {
  cnt=num=0;
  FILE *fp=fopen("/tmp/a.c", "w");  //clear it
  remove("/tmp/a.so");  //delete it, 32bit can't use 64bit .so
  fclose(fp);
  while(fgets(str, N, stdin)){
   printf(">> \n");
   if(str[0]=='i'&&str[1]=='n'&&str[2]=='t'){
     int i=0;
     for(int j=4;str[j]!=' ' && str[j]!='(';j++,i++){
       fun[num].name[i]=str[j];
     }
     fun[num].name[i]='\0';
     bool flag=false;
     for(i=0;i<num;i++){
      if(strcmp(fun[num].name,fun[i].name)==0){
        flag=true;
      }
     }
     num++;
     if(flag){
      printf("Error:You have defined this function\n");
      continue;
     }
     FILE *fp=fopen("/tmp/a.c", "a");
     if(!fp)
       assert(0);
     fprintf(fp,"%s", str);
     fclose(fp);
   }
   else {
     expr[0]='\0';  //clear
     name[0]='\0';
     str[strlen(str)-1]='\0';
     sprintf(expr,"int __expr_wrap_%d() { return (%s);}\n", cnt, str);
     sprintf(name,"__expr_wrap_%d",cnt);
     cnt++;
     FILE *fp=fopen("/tmp/a.c", "a");
     if(!fp)
       assert(0);
     fprintf(fp,"%s", expr);
     fclose(fp);
     if(sizeof(void *)==8)
      system("gcc /tmp/a.c -shared -fPIC -o /tmp/a.so -ldl");
     else
      system("gcc /tmp/a.c -m32 -shared -fPIC -o /tmp/a.so -ldl");
     void *so = dlopen("/tmp/a.so", RTLD_LAZY);
     int (*func)()=dlsym(so,name);
     int value=func();
     printf("  %s = %d.\n", str, value);
     dlclose(so);
   }
  }
  return 0;
}
