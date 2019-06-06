#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *copy;
char filename[80], sha[100];
void print_sha1sum(int size, int start){
  int fd=open(filename, O_CREAT|O_TRUNC|O_RDWR,S_IRWXU);
  write(fd, copy+start, size);
  close(fd);
  memset(sha,0,sizeof(sha));
  strcpy(sha, "sha1sum ");
  strcpy(sha, filename);
  system(sha);
}
int judge(int x){
  for(int i=x;i<x+8;i++){
    if(copy[i]=='~')
      return 0;
  }
  return 1;
}

int findname(int x){
  int temp=x;
  if(judge(temp)){
    memset(filename, 0, sizeof(filename));
    int i=0;
    while(copy[temp]!=0x20){
      filename[i]=copy[temp];
      temp++;
      i++;
    }
    filename[i]='\0';
    strcat(filename,".bmp");
    return 1;
  }
  int tempp=x-32;
  int cnt=1;
  while(1){
    if(copy[tempp]==(char)cnt){
      tempp-=32;
      cnt++;
    }
    else if(copy[tempp]==((char)cnt|(char)0x40)){   //this is the end
      break;
    }
    else return 0;    //have some mistakes
  }
  for(int i=28;i<32;i++){
    if(copy[tempp+i]!=(char)0xff)
      return 0;
  }
  memset(filename, 0, sizeof(filename));    //now we can recover long name
  int finish=0;
  int i=0;
  for(int j=x-32;j>=tempp;j-=32){
    temp=j+0x1;
    for(int k=0;k<5;k++){    //first part
      filename[i]=copy[temp];
      if(copy[temp]==0x0&&copy[temp+1]==0x0){
        finish=1;
        break;
      }
      i++, temp+=2;
    }
    if(finish)
      break;
    
    temp=j+0xe;
    for(int k=0;k<6;k++){    //second part
      filename[i]=copy[temp];
      if(copy[temp]==0x0&&copy[temp+1]==0x0){
        finish=1;
        break;
      }
      i++, temp+=2;
    }
    if(finish)
      break;

    temp=j+0x1c;
    for(int k=0;k<2;k++){    //third part
      filename[i]=copy[temp];
      if(copy[temp]==0x0&&copy[temp+1]==0x0){
        finish=1;
        break;
      }
      i++, temp+=2;
    }
    if(finish)
      break;    
  }
  return 1;
}

int main(int argc, char *argv[]) {
  int fd = open("filesystem/fs.img", O_RDONLY);
  if(fd==-1)
    printf("nmsl\n");
  
  copy=(char *)mmap(NULL, 2<<29, PROT_READ, MAP_SHARED, fd, 0); //512mb
  close(fd);
  short sector_bit=*(short *)&copy[0xb];
  int cluster_sector_num=(int)copy[0xd];
  int sector_num = *(int *)&copy[0x20];
  
  int end = sector_bit * sector_num;

  int cluster_start=*(int *)&copy[0x2c];
  int fat_start = *(short *)&copy[0xe];
  int fat_num=*(int *)&copy[0x10];
  int fat_sector_num=*(int *)&copy[0x24];

  int start=(fat_start+fat_num*fat_sector_num+(cluster_start-2)*cluster_sector_num)*sector_bit;

  /*printf("%d\n",sector_bit);
  printf("%d\n",cluster_sector_num);
  printf("%d\n",sector_num);
  printf("%08x\n", end);
  printf("%d\n", cluster_start);
  printf("%d\n", start);*/
  for(int i=start;i<end;i++){
    if(copy[i]=='B'&&copy[i+1]=='M'&&copy[i+2]=='P'){
      int base=i-8;
      if(copy[base]==0xe5)    //have been delete
        continue;

      int high_c=*(unsigned short *)&copy[base+0x14];
      int short_c=*(unsigned short *)&copy[base+0x1a]; 
      int file_cluster= (high_c<<16)+short_c;

      if(file_cluster<cluster_start)      //wrong cluster
        continue;

      if(!findname(base))
        continue;
      
      int file_address = start+(file_cluster-cluster_start)*cluster_sector_num*sector_bit;
      int file_size=*(int *)&copy[base+0x1c];

      print_sha1sum(file_size, file_address);
    }
  }
  return 0;
}
