#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
struct Node{
  char name[80];
  double time;
}func[3000];
int fildes[2];
char mem[10000];
char name[80];
int cunt;
double total;
void add(char *str, double time){
  total+=time;
  for(int i=0;i<cunt;i++){
    if(strcmp(str,func[i].name)==0){
      func[i].time+=time;
      return;
    }
  }
  strcpy(func[cunt].name,str);
  func[cunt++].time=time;
}
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
  int cunt0=0;
  char temp[80];
  for(;str[i]!='.';i--,cunt0++){
    temp[cunt0]=str[i];
  }
  double res1=0,res2=0;
  for(int j=0;j<cunt0;j++){
    res1+=temp[j]-'0';
    res1/=10.0;
  }
  cunt0=0;
  i--;
  for(;str[i]!='<';i--,cunt0++){
    temp[cunt0]=str[i];
  }
  for(int j=cunt0-1;j>=0;j--){
    res2=res2*10.0+temp[j]-'0';
  }
  //printf("%lf\n",res1+res2);
  add(name, res1+res2);
}

void print(){
	printf("\033[2J"); //清屏 
	printf("\033[0;0H");//设置光标位置
  for(int i=0; i < cunt; i++){
    printf("name:%-16s\ttime:%.6lf\t%.2lf%%\n", func[i].name,func[i].time,func[i].time*100.0/total);
  }
}
int main(int argc, char *argv[]) {
  cunt=0;
  total=0;
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
    while(fgets(mem, 10000, stdin)){
      printf("%s",mem);
      analysis(mem); 
      print();
      usleep(40000);      //to print
      /*if(i%10==0)
        system("reset");*/
    }
  }
  return 0;
}
