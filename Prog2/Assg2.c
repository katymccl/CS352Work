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
#include <curses.h>
 


//extern char** environ;
/* custom versions of exit, cd, and pwd*/

char *customNames[] = {
	"cd",
	"pwd",
	"exit"
};

int (*customFunc[]) (char **) = {
	&k_cwd, //addresses of each function
	&k_pwd,
	&k_exit
};

char **history ;
int numCommands = 0; 

int main(){

	user = getenv("USER"); /* gets the current user for the session*/
	if (getcwd(currentWD, sizeof(currentWD)) != NULL){
   		#if DEBUG
       		fprintf(stdout, "Starting in %s\n", currentWD);
   		#endif
   	}
   	else{
       perror("getcwd() error");
	}
	int status; 
	char *line;
	char **args;
	do{
		printf("$> ");
		line = readLine();
	
		if (line == NULL){
			if (feof(stdin)){
				return EXIT_SUCCESS;
			}
			else if (ferror(stdin)){
				#if DEBUG
				fprintf(stderr, "Error! End of file not reached\n");
				#endif
				fclose(stdin);
			}
			else{
				status = 1;
			}
		}
		else{ 
			args = parseLine(line);
			status = parseArgs(args); /*determines exit status*/
			free(args);
		}
		free(line);
	
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

	
	char *command = args[0]; /* command */

	if (pid < 0){
		fprintf(stderr, "Error: fork failed\n");
		return 1;
	}else if (pid == 0){
		if (strcmp(command, "calc") == 0){
			command = strcat(currentWD, "/calc");
		}
		if (strcmp (command, "listf")  == 0){
			command = strcat(currentWD, "/listf");
			/* parse list f arguments here */
			/* run list f*/
		}
		if (execvp(command, args) == -1){
		 	perror("Error: Command Not Found");
		 	#if DEBUG
		 	fprintf(stderr, "command: %s\n", command);
		 	#endif
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
	char home[1024];

	char *defaultChar = "~";
	strcpy(home, "/home/");
	strcat(home, user);

	if (args[1] == NULL){
		#if DEBUG
			perror("No directory given. Changing to default.");
		#endif

		if (chdir(home) !=0){
			perror("Error with cwd ");
		}
	}
	else if (strcmp(args[1], defaultChar) == 0){
		if (chdir(home) !=0){
			perror("Error with cwd ");
		}
	}
	else {
		if (chdir(args[1]) !=0){
			perror("Error with cwd ");
		}
	}


   if (	getcwd(current, sizeof(current)) != NULL){
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

	