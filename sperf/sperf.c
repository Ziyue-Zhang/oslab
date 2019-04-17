#include <stdio.h>
#include <unistd.h>
int fildes[2];
int main(int argc, char *argv[]) {
  for(int i = 1; i < argc; ++i){
	  printf("%s\n", argv[i]);
  } 
  if(pipe(fildes)!=0){
    printf("error!\n");
    return -1;
  }
  int pid = fork();
  if(pid==0){
    printf("This is son\n");
  }
  else{
    printf("this is father\n");
  }
  return 0;
}
