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
  printf(">> ");
  while(fgets(str, N, stdin)){
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
      printf("Error:You have defined this function.\n");
      printf(">> ");
      continue;
     }
     FILE *fp=fopen("/tmp/a.c", "a");
     if(!fp)
       assert(0);
     fprintf(fp,"%s", str);
     fclose(fp);
     printf("  Added: %s",str);
   }
   else {
	   printf("nmsl\n");
     bool flag=false;
     for(int i=0;i<strlen(str);i++){
      if((str[i]>='a'&&str[i]<='z')||(str[i]>='A'&&str[i]<='Z')||str[i]=='_') {
       if(str[i+1]=='\0'){
		   flag = true;
		   break;
       }
       else if(str[i+1]==' '){
         int j=i+1;
		 while(str[j]==' '){
			 j++;
		 }
		 if(str[j]!='('){
			flag=true;
			break;
		 }
       }       
      }
     }
     if(flag){
     printf("  Compile Error.\n");
     printf(">> ");
     continue;
     }
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
     printf("  (%s) = %d.\n", str, value);
     dlclose(so);
   }
   printf(">> ");
  }
  return 0;
}
