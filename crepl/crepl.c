#include <dlfcn.h>
#include <stdio.h>
#include <assert.h>
#define N 10000
int cnt;
char str[N];
char expr[N];
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
     sprintf(expr,"int __expr_wrap_%d() { return (%s);}", cnt, str);
     cnt++;
     FILE *fp=fopen("/tmp/a.c", "a");
     if(!fp)
       assert(0);
     fprintf(fp,"%s", expr);
     fclose(fp);
   }
  }
  return 0;
}
