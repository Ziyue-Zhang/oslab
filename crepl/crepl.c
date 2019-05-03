#include <dlfcn.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#define N 10000
int cnt;
char str[N];
char expr[N];
char name[N];
int main(int argc, char *argv[]) {
  cnt=0;
  FILE *fp=fopen("/tmp/a.c", "w");  //clear it
  fclose(fp);
  while(fgets(str, N, stdin)){
   if(str[0]=='i'&&str[1]=='n'&&str[2]=='t'){
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
     system("gcc /tmp/a.c -shared -fPIC -o /tmp/a.so -ldl");
     void *so = dlopen("/tmp/a.so", RTLD_LAZY);
     int (*func)()=dlsym(so,name);
     int value=func();
     printf(">> %s = %d.\n", str, value);
     dlclose(so);
   }
  }
  return 0;
}
