#include <dlfcn.h>
#include <stdio.h>
int main(int argc, char *argv[]) {
  FILE *fp=fopen("/tmp/a.c", "w");
  fprintf(fp,"nmsl\n");
  fclose(fp);
  return 0;
}
