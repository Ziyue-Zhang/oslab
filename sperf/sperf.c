#include <stdio.h>

int main(int argc, char *argv[]) {
  int pipe(int fildes[2]);
  for(int i = 1; i < argc; ++i){
	  printf("%s\n", argv[i]);
  } 
  if(pipe(fildes)!=0){
    printf("error!\n");
    return -1;
  }
  pid = fork();
  if(pid==0){
    printf("This is son\n");
  }
  else{
    printf("this is father\n");
  }
  return 0;
}
