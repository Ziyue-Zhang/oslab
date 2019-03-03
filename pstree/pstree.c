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
	char ppid[80];
	char name[80];
}node[N];
int size;
int root;

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
				//printf("%s\n", entry->d_name);
				test(entry->d_name); 
			}
		}
		else {
			if(strcmp("status", entry->d_name) == 0) {
				char filename[80] = "/proc/";
				strcat(filename, name);
				strcat(filename, "/status");
				//printf("%s\n", filename);
				FILE *fp = fopen(filename, "r");
				if(fp) {
					char temp[80];
					while(strcmp(temp, "Name:") != 0)
						fscanf(fp, "%s", temp);
					fscanf(fp, "%s", temp);
					strcpy(node[size].name, temp);
					while(strcmp(temp, "Pid:") != 0)
						fscanf(fp, "%s", temp);
					fscanf(fp, "%s", temp);
					strcpy(node[size].pid, temp);
                    if(strcmp(temp, "1") == 0)
						root = size;
					while(strcmp(temp, "PPid:") != 0)
						fscanf(fp, "%s", temp);
					fscanf(fp, "%s", temp);
					strcpy(node[size++].ppid, temp);
				}
				else {
					printf("wrong\n");
				}
			}
		}
	}
	chdir("..");
	closedir(dp);
}

void printtree(int fa, int len) {
	for(int i = 0; i < len; i++) {
		if(i == strlen(node[root].name) + 1)
			printf("│");
		else
			printf(" ");
	}
	printf("%s\n", node[fa].name);
	len += strlen(node[fa].name);
	for(int i = 0; i < size; i++) 
		if(strcmp(node[fa].pid, node[i].ppid) == 0) {
			printtree(i, len);
		}
	}
}
int main(int argc, char *argv[]) {
  test("/proc");
  //for(int i = 0; i < size; i++)
	//printf("%s %s %s\n", node[i].pid, node[i].name, node[i].ppid);
  printtree(root, 0);
  return 0;
}
