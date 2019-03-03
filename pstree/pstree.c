#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>


struct Node	{
	int pid;
	char* name;
	int ppid;
	int son;
};

void test(char *dir) {
	DIR *dp;
	struct dirent *entry;
	struct stat statbuf;
	//char * name = dir;
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
			if(strcmp("ststus", entry->d_name) == 0) {
				printf("ddhf\n");
			}
		}
	}
	chdir("..");
	closedir(dp);
}
int main(int argc, char *argv[]) {
  test("/proc:");
  return 0;
}
