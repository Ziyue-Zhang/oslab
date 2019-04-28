#include <stdio.h>
#include <unistd.h>
int fildes[2];
char *myarg[100]={"strace", "-T"};
int main(int argc, char *argv[]) {
  for(int i = 1; i < argc; ++i){
	  myarg[i + 1] = argv[i];
  } 
  if(pipe(fildes)!=0){
    printf("error!\n");
    return -1;
  }
  int pid = fork();
  if(pid==0){
    printf("This is son\n");
    //dup2(fildes[1],2);
    execve("/usr/bin/strace", myarg, NULL);
    printf("can you see me?\n");
  }
  else{
    printf("this is father\n");
    /*char str[1000000];
    while(1){
      int num = read(fildes[0],str,1000000);
      for(int i = 0; i < num; i++)
        printf("%c", str[i]);
    }*/
  }
  return 0;
}
