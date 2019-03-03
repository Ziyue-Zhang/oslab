#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#define N 10000

struct Node	{
	char pid[80];
	char name[80];
	char ppid[80];
	int son;
}node[N];
int size;

void test(char *dir) {
	DIR *dp;
	struct dirent *entry;
	struct stat statbuf;
	char * name = dir;
	if((dp = opendir(dir)) == NULL) {
		printf("Cannot open dir: %s\n", dir);
		return;
	}
	chdir(dir);
	while ((entry = readdir(dp)) != NULL) {
		lstat(entry->d_name, &statbuf);
		if(S_ISDIR(statbuf.st_mode)) {
			if(entry->d_name[0] >= '0' && entry->d_name[0] <= '9') {
				printf("%s\n", entry->d_name);
				test(entry->d_name); 
			}
		}
		else {
			if(strcmp("status", entry->d_name) == 0) {
				char filename[80] = "/proc/";
				strcat(filename, name);
				strcat(filename, "/status");
				printf("%s\n", filename);
				FILE *fp = fopen(filename, "r");
				if(fp) {
					char temp[80];
					while(strcmp(temp, "Name:") != 0)
						fscanf(fp, "%s", temp);
					fscanf(fp, "%s", temp);
					strcpy(node[size++].name, temp);
				}
			}
		}
	}
	chdir("..");
	closedir(dp);
}
int main(int argc, char *argv[]) {
  test("/proc");
  for(int i = 0; i < size; i++)
	  printf("%s\n", node[i]);
  return 0;
}
