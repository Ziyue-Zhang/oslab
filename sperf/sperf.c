#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
int fildes[2];
char mem[1000];
int main(int argc, char *argv[]) {
  if(pipe(fildes)!=0){
    printf("error!\n");
    return -1;
  }
  int pid = fork();
  if(pid==0){
    char **myarg = (char **)malloc((argc+2)*sizeof(char *));
    myarg[0]="strace";
    myarg[1]="-T";
    for(int i = 1; i < argc; ++i){
	  myarg[i + 1] = argv[i];
    } 
    myarg[argc+1]=NULL;
     /*for(int i = 0; i <= argc; ++i){
	  printf("%s ",myarg[i]);
    } */
    printf("This is son\n");
    close(fildes[0]);
    dup2(fildes[1],STDERR_FILENO);
    int fd = open("/dev/null", O_WRONLY);
    dup2(fd, STDOUT_FILENO);
    execvp("strace", myarg);
    printf("can you see me?\n");
  }
  else{
    printf("this is father\n");
    close(fildes[1]);
    dup2(fildes[0], STDIN_FILENO);
    while(fgets(mem, 1000, stdin)){
      printf("%s\n",mem);
    }
  }
  return 0;
}
