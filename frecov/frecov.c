#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdio.h>

char *copy;
char shortname[16];
//int print_sha1sum(char *name, int size, int start){
//}

int main(int argc, char *argv[]) {
  int fd = open("filesystem/fs.img", O_RDONLY);
  if(fd==-1)
    printf("nmsl\n");
  
  copy=(char *)mmap(NULL, 2<<29, PROT_READ, MAP_SHARED, fd, 0); //512mb
  short sector_bit=*(short *)&copy[0xb];
  int cluster_sector_num=(int)copy[0xd];
  int sector_num = *(int *)&copy[0x20];
  
  int end = sector_bit * sector_num;

  int cluster_start=*(int *)&copy[0x2c];
  int fat_start = *(short *)&copy[0xe];
  int fat_num=*(int *)&copy[0x10];
  int fat_sector_num=*(int *)&copy[0x24];

  int start=(fat_start+fat_num*fat_sector_num+(cluster_start-2)*cluster_sector_num)*sector_bit;

  printf("%d\n",sector_bit);
  printf("%d\n",cluster_sector_num);
  printf("%d\n",sector_num);
  printf("%08x\n", end);
  printf("%d\n", cluster_start);
  printf("%d\n", start);
  int cunt=0;
  for(int i=start;i<end;i++){
    if(copy[i]=='B'&&copy[i+1]=='M'&&copy[i+2]=='P'){
      int base=i-8;
      if(copy[base]==0xe5)    //have been delete
        continue;
      int high_c=*(unsigned short *)&copy[base+0x14];
      int short_c=*(unsigned short *)&copy[base+0x1a]; 
      int file_cluster= (high_c<<16)+short_c;
      int file_address = start+(file_cluster-cluster_start)*cluster_sector_num*sector_bit;
      int file_size=*(int *)&copy[base+0x1c];

      
    }
  }
  printf("%d\n", cunt);
  return 0;
}
