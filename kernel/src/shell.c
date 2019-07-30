#include<vfs.h>

char pwd[200]="/";
char path1[200];
char path2[200];
/*void echo_task(void *name){
  device_t *tty = dev_lookup(name);
  while(1){
    char line[128], text[128];
    sprintf(text, "(%s) $ ", name); tty_write(tty, 0, text, strlen(text));
    int nread = tty->ops->read(tty, 0 ,line, sizeof(line));
    line[nread - 1] = '\0';
    sprintf(text, "Echo: %s.\n", line); tty_write(tty, 0, text,strlen(text));
  }
}*/
void get_path(char *pwd, char *path){
  if(path[0]!='/'){
    path1[0]='0';
    strcpy(path1,pwd);
    strcpy(path1,"/");
    strcpy(path1,path);
  }
  else{
    strcpy(path1,path);
  }
}
void command_ls(char *line){
  int id=vfs_lookup(pwd);
  int son=vinode[id].son;
  int n=0;
  while(1){
      n+=sprintf(line+n, "%s",vinode[son].name);
      if(vinode[son].nxt==-1)
        break;
      n+=sprintf(line+n, "    ");
      son=vinode[son].nxt;
  }
  n+=sprintf(line+n, "\n");
}

void command_pwd(char *line){
  sprintf(line, "%s\n", pwd);
}

void command_cd(char *line,char *text){
  int i=0;
  while(line[i]){
    if(line[i]==' ')
      break;
    i++;
  }
  i++;
  if(strcmp(line+i,".")==0){
    sprintf(line, "\n");
    return;
  }
  else if(strcmp(line+i,"..")==0){
    if(strcmp(pwd,"/")==0){
      sprintf(line, "\n");
      return;
    }
    else{
      int j=strlen(pwd)-1;
      while(1){
        if(pwd[j]=='/')
          break;
        j--;
        pwd[j]='0';
        sprintf(line, "\n");
        return;
      }
    }
  }
  else{
    char temp[200];
    strcpy(temp,pwd);
    get_path(temp, line+i);
    if(vfs_lookup(temp)!=-1){
      get_path(pwd, line+i);
    }
    pwd[0]='\0';
    strcpy(pwd,path1);
    sprintf(line, "\n");
  }
}

void terminal_task(void *name){
  device_t *tty = dev_lookup(name);
  char line[128], text[128];
  while(1){
    sprintf(text, "(%s) %s$ ", name,pwd); 
    tty->ops->write(tty, 0, text, strlen(text));
    int nread = tty->ops->read(tty, 0 ,line, sizeof(line));
    line[nread - 1] = '\0';
    if(strncmp(line,"ls",strlen("ls"))==0){
      command_ls(text);
    }
    else if(strcmp(line,"pwd")==0){
      command_pwd(text);
    }
    else
      sprintf(text, "Echo: %s.\n", line); 
    tty_write(tty, 0, text,strlen(text));
  }
}
