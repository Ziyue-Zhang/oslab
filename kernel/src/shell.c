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
      if(vinode[son].link_inode==son)
        n+=sprintf(line+n, "%s",vinode[son].name);
      else{
        n+=sprintf(line+n, "%s -> %s",vinode[son].name,vinode[vinode[son].link_inode].name);
      }
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
  int mode=vfs_mkdir(path2);
  if(mode==-1){
    n+=sprintf(text+n, "dir %s exists, make it fail!\n",line+i);
  }
  else
    n+=sprintf(text+n, "make %s successful!\n",line+i);
}
void command_rmdir(char *line, char *text){
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
  //printf("%s\n",path2);
  int mode=vfs_rmdir(path2);
  if(mode==0){
    n+=sprintf(text+n, "remove %s successful!\n",line+i);
  }
  else if(mode==1){
    n+=sprintf(text+n, "Can't find %s, remove it fail!\n",line+i);
  }
  else if(mode==2){
    n+=sprintf(text+n, "%s isn't a dir, remove it fail!\n",line+i);
  }
}
void command_link(char *line, char *text){
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
  int j=i;
  while(line[j]){
    if(line[j]==' ')
      break;
    j++;
  }
  char temp1[200],temp2[200];
  strncpy(temp1,line+i,j-i);
  strcpy(temp2,line+j+1);
  if(temp1[strlen(temp1)-1]=='/')
    temp1[strlen(temp1)-1]='\0';
  if(temp2[strlen(temp2)-1]=='/')
    temp2[strlen(temp2)-1]='\0';
  strcpy(path1,pwd);
  strcpy(path2,pwd);
  strcat(path1,temp1);
  strcat(path2,temp2);
  int mode=vfs_link(path1,path2);
  if(mode==-2){
    n+=sprintf(text+n, "wrong oldpath %s, link fail!\n",path1);
  }
  else if(mode==-1){
    n+=sprintf(text+n, "wrong newpath %s, link fail!\n",path2);
  }
  else  
    n+=sprintf(text+n, "link %s -> %s successful!\n",path1,path2);
}
void command_unlink(char *line, char *text){
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
  char temp[200];
  strcpy(temp,line+i);
  if(temp[strlen(temp)-1]=='/')
    temp[strlen(temp)-1]='\0';
  strcpy(path1,pwd);
  strcat(path1,temp);
  int mode=vfs_unlink(path1);
  if(mode==-1){
    n+=sprintf(text+n, "wrong path %s, unlink fail!\n",path1);
  }
  else if(mode==0){
    n+=sprintf(text+n, "no link exists, nulink fail!\n");
  }
  else  
    n+=sprintf(text+n, "unlink %s successful!\n",path1);
}
void command_cat(char *line, char *text){
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
  char temp[200];
  strcpy(temp,line+i);
  if(temp[strlen(temp)-1]=='/')
    temp[strlen(temp)-1]='\0';
  strcpy(path1,pwd);
  strcat(path1,temp);
  int id=vinode_find(path1);
  printf("%d\n",id);
  if(id==-1){
    n+=sprintf(text+n, "wrong path %s, cat fail!\n",path1);
  }
  else if(vinode[id].type==DIR){
    n+=sprintf(text+n, "this is a dir, cat fail!\n",path1);
  }
  else if(vinode[id].type==PROC){
    printf("nmsl\n");
    proc_read(vinode[id].inode,0,text);
  }
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
      command_rmdir(line,text); 
    }
    else if(strncmp(line,"link",strlen("link"))==0){
      command_link(line,text); 
    }
    else if(strncmp(line,"unlink",strlen("unlink"))==0){
      command_unlink(line,text); 
    }
    else if(strncmp(line,"cat",strlen("cat"))==0){
      command_cat(line,text); 
    }
    else if(strncmp(line,"echo",strlen("echo"))==0){
      sprintf(text, "%s\n", line+5); 
    }
    else
      sprintf(text, "Echo: %s.\n", line); 
    tty_write(tty, 0, text,strlen(text));
  }
}
