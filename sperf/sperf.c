#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
int fildes[2];
char mem[100000];
void analysis(char *str){

}

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
    //printf("This is son\n");
    close(fildes[0]);
    dup2(fildes[1],STDERR_FILENO);
    int fd = open("/dev/null", O_WRONLY);
    dup2(fd, STDOUT_FILENO);
    execvp("strace", myarg);
    printf("can you see me?\n");
  }
  else{
    //printf("this is father\n");
    close(fildes[1]);
    dup2(fildes[0], STDIN_FILENO);
    usleep(20000);
    while(fgets(mem, 100000, stdin)){
      printf("%s",mem);
      analysis(mem);
      usleep(20000);      //avoid this programe stop
    }
  }
  return 0;
}
