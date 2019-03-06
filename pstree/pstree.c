#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdbool.h>
#define N 10000

struct Node	{
	char pid[80];
	char ppid[80];
	char name[80];
}node[N];
int size;
int root;

void test(char *dir, char *filename) {
	DIR *dp;
	struct dirent *entry;
	struct stat statbuf;
	if((dp = opendir(dir)) == NULL) {
		printf("Cannot open dir: %s\n", dir);
		return;
	}
	chdir(dir);
	while ((entry = readdir(dp)) != NULL) {
		lstat(entry->d_name, &statbuf);
		if(S_ISDIR(statbuf.st_mode)) {
			if(strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0)
				continue;
			if((entry->d_name[0] >= '0' && entry->d_name[0] <= '9') || strcmp("task", entry->d_name) == 0) {
				//printf("%s %s\n", name, entry->d_name);
				char name[80];
				strcpy(name, filename);
				strcat(name, "/");
				strcat(name, entry->d_name);
				test(entry->d_name, name);
			}
		}
		else {
			if(strcmp("status", entry->d_name) == 0) {
				strcat(filename, "/status");
				//printf("%s\n", filename);
				int i = 0;
				bool flag = false;
				for(; i < strlen(filename); i++)
					if(filename[i] == 't' &&
					   filename[i+1] == 'a' &&
					   filename[i+2] == 's' &&
					   filename[i+3] == 'k') {
						flag = true;
						break;
					}
				char cmp[80];
				if(flag){
					i-=2;
					while(i--)
						if(filename[i] == '/')
							break;
					i++;
					int j = 0;
					for(; filename[i]!='/'; i++,j++) {
						cmp[j] = filename[i];
					}
					cmp[j] = '\0';
					//printf("%s\n", cmp);
				}
				FILE *fp = fopen(filename, "r");
				if(fp) {
					char temp0[80];
					char temp[80];
					while(strcmp(temp0, "Name:") != 0)
						fscanf(fp, "%s", temp0);
					fscanf(fp, "%[^\n]%*c", temp0);
					char *tempn = temp0;
					while(*tempn == '\t' || *tempn == 32)
						tempn++;
					//printf("%s\n", tempn);
					/*if(strcmp(temp,"gdbus")==0)
						printf("%s\n", filename);*/
					/*bool flag = true;
					for(int i = 0; i < size; i++)
						if(strcmp(node[i].name, temp) == 0) {
							flag= false;
							break;
						}
					if(!flag){
						break;
					}*/
					while(strcmp(temp, "Pid:") != 0)
						fscanf(fp, "%s", temp);
					fscanf(fp, "%s", temp);
					if(flag && strcmp(cmp, temp) == 0)
						break;
					if(!flag)
						strcpy(node[size].name, tempn);
					else{
						strcpy(node[size].name, "{");
						strcat(node[size].name, tempn);
						strcat(node[size].name, "}");
					}
					strcpy(node[size].pid, temp);
                    if(strcmp(temp, "1") == 0)
						root = size;
					while(strcmp(temp, "PPid:") != 0)
						fscanf(fp, "%s", temp);
					fscanf(fp, "%s", temp);
					if(!flag)
						strcpy(node[size].ppid, temp);
					else
						strcpy(node[size].ppid, cmp);
					size++;
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

void printtree(int fa, int len, bool first, bool single) {
	if(!first) {
		for(int i = 0; i < len; i++) {
		/*	if(i == strlen(node[root].name) + 2)
				printf("│");
			else
				printf(" ");*/
			printf(" ");
		}
	}
	if(fa != root) {
		if(single)
			printf("───%s", node[fa].name);
		else
			printf(" └─%s", node[fa].name);
	}
	else
		printf("%s", node[fa].name);
	if(fa == root)
		len -= 3;
	len += strlen(node[fa].name);
	int cunt = 0;
	for(int i = 0; i < size; i++) {
		if(strcmp(node[fa].pid, node[i].ppid) == 0) {
				cunt++;
			}
	}
	bool flag = true;
	for(int i = 0; i < size; i++) { 
		if(strcmp(node[fa].pid, node[i].ppid) == 0) {
			if(cunt == 1)
				printtree(i, len + 3, flag, true);
			else
				printtree(i, len + 3, flag, false);
			if(flag)
				flag = false;
		}
	}
	if(flag)
		printf("\n");
}
int main(int argc, char *argv[]) {
  test("/proc","/proc");
  //for(int i = 0; i < size; i++)
	//printf("%s %s %s\n", node[i].pid, node[i].name, node[i].ppid);
  printtree(root, 0, true, false);
  return 0;
}
