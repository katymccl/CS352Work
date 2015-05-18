/*
	listf.c
	Author: Kathryn McClintic
	Last modified: May 17th, 2015
	Purpose: implements  a custom ls unix command with additional features
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>  
#include <pwd.h> 
#include <unistd.h>
#include <grp.h> 
#include <fcntl.h>
#include <langinfo.h>
#include <stdint.h>
#include <dirent.h>
#include <sys/param.h> 
#include <time.h>
#include "listf.h"


/* Profile for listf flags */
typedef struct{
	bool l; /* long */
	bool a; /* access time */
	bool m; /* mod time */
	bool c; /* creation time */
}Profile;

/* Main ls loop */
int main(int argc, char **argv){
	int i = 0;
	int j = 0;	
	int k = 0;
	int l = 0;
	int numEntries = 0;
	char *flag;

	/* data structures for ls info */
	struct tm timeInfo;
	char timeBuffer[BUF_SIZE];

	struct stat data;
	struct passwd pswd;
	struct passwd *ownerData;
	struct group grp;
	struct group *groupData;

	char buf[256];

	struct dirent **entries; /* an array of structs of directory entries */
	char *dir[MAXDIR]; 
	Profile list_prof = {false,false,false,false}; 

	if (getcwd(currentWD, sizeof(currentWD)) == NULL){
		perror("getcwd() error");
   	}
 

	/* Parse flags */
	for (j = 1; j < argc; j++){
		if(strstr (argv[j], "-")){
			flag = strstr (argv[j], "-");
			for (l = 0; l < strlen(flag); l++){
				switch (flag[l])
				{
					case 'l':
						list_prof.l = true;
						break;
					case 'a':
						list_prof.a = true;
						break;
					case 'm':
						list_prof.m = true;
						break;
					case 'c':
						list_prof.c = true;
						break;
					case '-': 
						break;
				}	
				k++;
			} 
		}
		else{
			dir[i] = malloc(BUF_SIZE);
			if (argv[j][0] != '/'){
				/* support for relative path */
				sprintf(dir[i], "%s/%s", currentWD,argv[j]);
			}
			else{
				sprintf(dir[i], "%s",argv[j]);
			}
			i++;			
		}
	}

	if (i == 0){
		dir[0] = currentWD;
		i = 1;
	}
	if (!list_prof.c && !list_prof.a && !list_prof.m){
		list_prof.m = true; /* setting default -m option */
	}


	/* Iterate through directory */
	for (k = 0; k < i; k++){
		chdir(dir[k]); /* to use stat function */
		numEntries = scandir (dir[k], &entries, entriesFilter, compareDir);
		#if DEBUG
			fprintf(stdout, "%d entries found\n", numEntries);
		#endif
		for (j = 0; j < numEntries; j++){
			if(list_prof.l){
				/* printing out times and permissions */
				if (stat(entries[j]->d_name, &data) == 0){
					fprintf(stdout, "%s %d ", getPermissions(data.st_mode), (int) data.st_nlink);
					if (!getpwuid_r(data.st_uid, &pswd, buf, sizeof(buf), &ownerData)){
						fprintf(stdout, " %s",pswd.pw_name);
					}
					else{
						fprintf(stdout, " %d", data.st_uid);
					}
					if(!getgrgid_r(data.st_gid, &grp, buf, sizeof(buf), &groupData)){
						fprintf(stdout, " %s",grp.gr_name);
					}
					else{
						fprintf(stdout, " %d",data.st_gid);
					}
					if (list_prof.m){
						localtime_r(&data.st_mtime, &timeInfo);
						strftime(timeBuffer, sizeof(timeBuffer), "M%x-%X", &timeInfo);
						fprintf(stdout, " %s", timeBuffer);
					}
					if (list_prof.a){
						localtime_r(&data.st_atime, &timeInfo);
						strftime(timeBuffer, sizeof(timeBuffer), "A%x-%X", &timeInfo);
						fprintf(stdout, " %s", timeBuffer);
					}
					if (list_prof.c){
						localtime_r(&data.st_ctime, &timeInfo);
						strftime(timeBuffer, sizeof(timeBuffer), "C%x-%X", &timeInfo);
						fprintf(stdout, " %s", timeBuffer);
					}
				}
				else{
					perror("error ");
				}
				fprintf(stdout, " %s", entries[j]->d_name);
				fprintf(stdout,"\n");
			}
			else{
				fprintf(stdout, " %s", entries[j]->d_name);
			}
		}
		fprintf(stdout,"\n");
	}
	return 1;
}

/* compareDir 
 * sorts directory entries alphabetically 
 * to be used in scandir as a sort function parameter */
int compareDir ( const struct dirent **a, const struct dirent **b)
{
  return strcasecmp ((*a)->d_name,(*b)->d_name);
}

/* entriesFiler
 * gets rid of hidden files from directory list 
 * to be used in scandir as a filter function parameter */
int entriesFilter(const struct dirent *entry){
	return entry->d_name[0] != '.';
}

/* getPermissions
 * retrieve permission bits from mode */
char *getPermissions(mode_t mode){

	char *permissions = malloc(BUF_SIZE);

	char fileType = 'o';
	if (S_ISREG(mode)){
		fileType = '-';  
	}
	if (S_ISLNK(mode)){
		fileType = 'l';  
	}
	if (S_ISDIR(mode)){
		fileType = 'd';  
	}

	sprintf(permissions, "%c%c%c%c%c%c%c%c%c%c", fileType,  
	mode & S_IRUSR ? 'r' : '-',  
	mode & S_IWUSR ? 'w' : '-',  
	mode & S_IXUSR ? 'x' : '-',  
	mode & S_IRGRP ? 'r' : '-',  
	mode & S_IWGRP ? 'w' : '-',  
	mode & S_IXGRP ? 'x' : '-',  
	mode & S_IROTH ? 'r' : '-',  
	mode & S_IWOTH ? 'w' : '-',  
	mode & S_IXOTH ? 'x' : '-');

	return permissions;  

}