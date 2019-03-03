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
	int son;
}node[N];
int size;
int cunt;

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
					strcpy(node[size].name, temp);
					while(strcmp(temp, "Pid:") != 0)
						fscanf(fp, "%s", temp);
					fscanf(fp, "%s", temp);
					strcpy(node[size++].pid, temp);
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
void match(char *dir) {
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
				match(entry->d_name); 
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
					while(strcmp(temp, "PPid:") != 0)
						fscanf(fp, "%s", temp);
					fscanf(fp, "%s", temp);
					printf("%s\n", temp);
				//	if(strcmp(node[cunt].pid, temp)!=0)
				//		printf("wrong\n");
				    int i = 0;
					for(; i < size; i++)
						if(strcmp(node[i].pid, temp) == 0)
							break;
					if(i < size) {
						node[i].son = cunt;
					}
					else
						printf("wrong\n");
					cunt++;
				}
			}
		}
	}
	chdir("..");
	closedir(dp);
}
int main(int argc, char *argv[]) {
  test("/proc");
  match("/proc");
 /* for(int i = 0; i < size; i++)
	  printf("%s %s %d\n", node[i].pid, node[i].name, node[i].son);*/
  return 0;
}
