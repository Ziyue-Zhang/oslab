#include <stdio.h>
#include <assert.h>

struct Node	{
	int pid;
	char* name;
	int ppid;
	int son;
}
int main(int argc, char *argv[]) {
  printf("Hello, World!\n");
  int i;
  for (i = 0; i < argc; i++) {
    assert(argv[i]); // always true
    printf("argv[%d] = %s\n", i, argv[i]);
  }
  assert(!argv[argc]); // always true
  return 0;
}
