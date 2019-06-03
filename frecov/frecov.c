#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdio.h>

char *copy;
char shortname[16];

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

  int start=(fat_start+fat_num*fat_sector_num)*sector_bit;

  printf("%d\n",sector_bit);
  printf("%d\n",cluster_sector_num);
  printf("%d\n",sector_num);
  printf("%d\n", end);
  printf("%d\n", cluster_start);
  printf("%d\n", start);

 
  return 0;
}
