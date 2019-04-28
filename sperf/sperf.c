#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
int fildes[2];
char mem[100000];
int main(int argc, char *argv[]) {
  char **myarg = (char **)malloc((argc+2)*sizeof(char *));
  myarg[0]="strace";
  myarg[1]="-T";
  for(int i = 1; i < argc; ++i){
	  myarg[i + 1] = argv[i];
  } 
  if(pipe(fildes)!=0){
    printf("error!\n");
    return -1;
  }
  int pid = fork();
  if(pid==0){
    close(fildes[0]);
    printf("This is son\n");
    FILE * fd = fopen("/dev/null", "a");
    dup2(fileno(fd), STDOUT_FILENO);
    fclose(fd);
    dup2(fildes[1],STDERR_FILENO);
    close(fildes[1]);
    execve("/usr/bin/strace", myarg, NULL);
    printf("can you see me?\n");
  }
  else{
    close(fildes[1]);
    printf("this is father\n");
    dup2(fildes[0], STDIN_FILENO);
    close(fildes[0]);
    while(fgets(mem, 100000, stdin)){
      printf("%s", mem);
    }
  }
  return 0;
}
