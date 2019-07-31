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
    path1[0]='\0';
    strcpy(path1,pwd);
    strcat(path1,path);
  }
  else{
    path1[0]='\0';
    strcpy(path1,path);
  }
}
void command_ls(char *line){
  path2[0]='\0';
  strcpy(path2,pwd);
  if(strcmp(path2,"/")!=0){
    int i=strlen(path2)-1;
    path2[i]='\0';
  }
  int id=vfs_lookup(path2);
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
  path2[0]='\0';
  strcpy(path2,pwd);
  if(strcmp(path2,"/")!=0){
    int i=strlen(path2)-1;
    path2[i]='\0';
  }
  sprintf(line, "%s\n", path2);
}

void command_cd(char *line,char *text){
  int i=0;
  while(line[i]){
    if(line[i]==' ')
      break;
    i++;
  }
  if(line[i]=='\0'){
    sprintf(text, "\n");
    return;
  }
  i++;
  if(strcmp(line+i,".")==0||strcmp(line+i,"./")==0){
    sprintf(text, "\n");
    return;
  }
  else if(strcmp(line+i,"..")==0||strcmp(line+i,"../")==0){
    if(strcmp(pwd,"/")==0){
      sprintf(text, "\n");
      return;
    }
    else{
      int j=strlen(pwd)-2;
      while(1){
        if(pwd[j]=='/')
          break;
        j--;
      }
      pwd[j+1]='\0';
      sprintf(text, "\n");
      return;
    }
  }
  else{
    int j=strlen(line)-1;
    if(line[j]=='/'){
      line[j]='\0';
    }
    get_path(pwd, line+i);
    //printf("%s\n",pwd);
    //printf("%s\n",line+i);
    //printf("%s\n",path1);
    int id=vfs_lookup(path1);
    if(id!=-1){
      if(vinode[id].type==DIR){
        pwd[0]='\0';
        //printf("%s\n",vinode[id].path);
        strcpy(pwd,vinode[id].path);
        sprintf(text, "\n");
        return;
      }
      else{
        sprintf(text, "Not a directory\n");
        return;
      }
    }
    sprintf(text, "wrong path!\n");
  }
}

void command_mkdir(char *line, char *text){
  int i=0;
  int n=0;
  while(line[i]){
    if(line[i]==' ')
      break;
    i++;
  }
  if(line[i]=='\0'){
    sprintf(text, "\n");
    return;
  }
  i++;
  for(int j=i;line[j]!='\0';j++){
    if(line[j]=='/'){
      sprintf(text, "dir name can't have '/'!\n");
      return;
    }
  }
  int j=strlen(line)-1;
  if(line[j]=='/'){
    line[j]='\0';
  }
  path2[0]='\0';
  strcpy(path2,pwd);
  strcat(path2,line+i);
  vfs_mkdir(path2);
  n+=sprintf(text+n, "mkdir %s successful!\n",line+i);
}
void command_rmdir(char *line){
  path2[0]='\0';
  strcpy(path2,pwd);
  if(strcmp(path2,"/")!=0){
    int i=strlen(path2)-1;
    path2[i]='\0';
  }
  int id=vfs_lookup(path2);
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

void terminal_task(void *name){
  device_t *tty = dev_lookup(name);
  char line[128], text[128];
  while(1){
    path2[0]='\0';
    strcpy(path2,pwd);
    if(strcmp(path2,"/")!=0){
      int i=strlen(path2)-1;
      path2[i]='\0';
    }
    sprintf(text, "(%s) %s$ ", name,path2); 
    tty->ops->write(tty, 0, text, strlen(text));
    int nread = tty->ops->read(tty, 0 ,line, sizeof(line));
    line[nread - 1] = '\0';
    if(strncmp(line,"ls",strlen("ls"))==0){
      command_ls(text);
    }
    else if(strcmp(line,"pwd")==0){
      command_pwd(text);
    }
    else if(strncmp(line,"cd",strlen("cd"))==0){
      command_cd(line,text);
    }
    else if(strncmp(line,"mkdir",strlen("mkdir"))==0){
      command_mkdir(line,text);
    }
    else if(strncmp(line,"rmdir",strlen("rmdir"))==0){
      sprintf(text, "%s\n", line+5); 
    }
    else if(strncmp(line,"echo",strlen("echo"))==0){
      sprintf(text, "%s\n", line+5); 
    }
    else
      sprintf(text, "Echo: %s.\n", line); 
    tty_write(tty, 0, text,strlen(text));
  }
}
