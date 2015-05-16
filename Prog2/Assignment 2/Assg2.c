/*
	Assg2.c
	Author: Kathryn McClintic
	Last modified: May 4th, 2015
	Purpose: main file 
*/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include "calc.h"
#include "Assg2.h"


//extern char** environ;
/* custom versions of exit, cd, and pwd*/

char *customNames[] = {
	"cwd",
	"pwd",
	"exit"
};

int (*customFunc[]) (char **) = {
	&k_cwd, //addresses of each function
	&k_pwd,
	&k_exit
};

int main(){

/*customFunc[0] = k_cwd;
customFunc[1]  = k_pwd;
customFunc[2] = k_exit;*/


	int status; 
	char *line;
	char **args;
	do{
		printf("$> ");
		line = readLine();
		args = parseLine(line);
		status = parseArgs(args); /*determines exit status*/
		fprintf(stdout, "status = %d\n", status);

		free(line);
		free(args);

	} while (status);

	return EXIT_SUCCESS;
}

char *readLine(){
	char *buf = malloc(sizeof(char) * BUF_SIZE);
	if (!buf){
		fprintf(stderr, "Error in malloc\n");
		exit(EXIT_FAILURE);
	}
	if (fgets(buf, BUF_SIZE, stdin) != NULL){ /* gets one line*/
		return strtok(buf, "\n");
	} 

	return NULL;
}

char **parseLine(char *line){
	char *token;
	char **args =  malloc(BUF_SIZE * sizeof(char*));

	int count = 0; 

	token = strtok(line, TOKEN_DELIM); 
	while (token!= NULL){

		args[count] = token;
		count++;

		token = strtok(NULL, TOKEN_DELIM);
		
		if (count >= BUF_SIZE){
			fprintf(stderr,"Error: no room in buffer");
			exit(EXIT_FAILURE);
		}
	}
	args[count] = NULL;
	return args;
}
/* char **args is just like an array of strings*/
int parseArgs(char **args){
	int i = 0;
	if (args[0] == NULL){
		fprintf(stderr,"error, empty command entered\n");
		return 1;
	}
	char *command = args[0]; /* command */
	for (i = 0; i < numCustom; i++){
		if(strcmp(args[0], customNames[i]) == 0){
			return (*customFunc[i]) (args);
		}
	}
	return runExternalCommands(args);
}

/*function: runExternalCommands
*/

int runExternalCommands(char **args){
	pid_t pid;
	int status;
	pid = fork();
	char *path = "cats";
	char *command = args[0]; /* command */

	if (pid < 0){
		fprintf(stderr, "Error: fork failed\n");
		return 1;
	}else if (pid == 0){
		puts(command);
		if (strcmp(command, "calc") == 0){
			puts(command);
			path = getenv("PATH");
			puts(path);
			strcat(path, ":/home/mcclink2/CS352/Assignment\ 2/calc");
					puts(path);
			if (setenv("PATH", path,1) == 0){
				puts ("hey");
			}
			//char *path = getenv("PATH");
			puts("CAT");

			command = "calc";
		}
		if (execvp(command, args) == -1){
			puts(command);
			puts(args[1]);
			perror("Error with external functions (possible mispelling of built-in ?)");
		}
		exit(EXIT_FAILURE);
	}
	else{
		waitpid(pid, &status, 0);
		return 1;
	}

	return 1;
}

/* function: k_pwd
 * returns 1 on success */
int k_pwd (char **args){
	char current[1024];
	if (getcwd(current, sizeof(current)) != NULL){
   	#if DEBUG
       fprintf(stdout, "%s", current);
   	#endif
   }
   	else{
       perror("getcwd() error");
	}
	return 1;
}

/* function: k_cwd
 * returns 1 on success */
int k_cwd(char **args){
	char current[1024];
	if (args[1] == NULL){
		chdir("/home"); //default
	}
	else {
		if (chdir(args[1]) !=0){
			perror("Error with cwd ");
		}
	}
	char *s_string = getcwd(current, sizeof(current));

   if (s_string!= NULL){
	   	#if DEBUG
	       fprintf(stdout, "cwd changed to %s\n", current);
	   	#endif
   }
   else{
       perror("getcwd() error");
	}
	return 1;
}

/*function: k_exit
 * returns status of 0 to go back to main */
int k_exit(char **args){
	return 0; /* on exit */
}

	