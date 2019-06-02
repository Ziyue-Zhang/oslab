#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdio.h>
char *copy;
int main(int argc, char *argv[]) {
  int fd = open("filesystem/fs.img", O_RDONLY);
  if(fd==-1)
    printf("nmsl\n");
  copy=(char *)mmap(NULL, 2<<29, PROT_READ, MAP_SHARED, fd, 0); //512mb
  short temp=*(short *)&copy[0xb];
  printf("%d\n",temp);
  return 0;
}
