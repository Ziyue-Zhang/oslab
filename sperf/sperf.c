#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
int fildes[2];
char mem[10000];
char name[80];

void analysis(char *str){
  int len=strlen(str)-1;
  if(str[len-1]!='>' || str[0]=='+'){
    return;
  }
  int i=0;
  for(;str[i]!='(';i++)
    name[i]=str[i];
  name[i]='\0';
  i = len;
  while(i){
    if('0'<=str[i] && str[i]<='9')
      break;
    i--;
  }
  int cunt=0;
  char temp[80];
  for(;str[i]!='.';i--,cunt++){
    temp[cunt]=str[i];
  }
  double res1=0,res2=0;
  for(int j=0;j<cunt;j++){
    res1+=temp[j]-'0';
    res1/=10.0;
  }
  cunt=0;
  i--;
  for(;str[i]!='<';i--,cunt++){
    temp[cunt]=str[i];
  }
  for(int j=cunt-1;j>=0;j--){
    res2=res2*10.0+temp[j]-'0';
  }
  printf("lf\n",res1+res2);
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
    int i=0;
    while(fgets(mem, 10000, stdin)){
      printf("%s",mem);
      analysis(mem); 
      i++;
      usleep(13140);      //to print
      /*if(i%10==0)
        system("reset");*/
    }
  }
  return 0;
}
