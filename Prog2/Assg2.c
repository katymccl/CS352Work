/*
	Assg2.c
	Author: Kathryn McClintic
	Last modified: May 17th, 2015
	Purpose: main file for command line interpreter
	Includes cd, pwd, and exit built in functions
	Uses execvp to call external functions including listf (custom ls) and calc (calculator)
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
#include <fcntl.h> 

/* custom versions of exit, cd, and pwd*/
char *customNames[] = {
	"cd",
	"pwd",
	"exit"
};

/* table of custom function addresses */
int (*customFunc[]) (char **) = {
	&k_cwd, //addresses of each function
	&k_pwd,
	&k_exit
};

bool redirectOut = false; // '<'
bool redirectIn = false; // '>'

char *fileIn;
char *fileOut;

int numCommands = 0; 
int count = 0;


/* Main */
int main(){
	int status; 
	char *line;
	char **args;

	user = getenv("USER"); /* gets the current user for the session*/
	if (getcwd(currentWD, sizeof(currentWD)) != NULL){
   		#if DEBUG
       		fprintf(stdout, "Starting in %s\n", currentWD);
   		#endif
   	}
   	else{
       perror("getcwd() error");
	}

	/* main shell loop */
	do{
		printf("$> ");
		line = readLine(); /* get line from user */
	
		if (line == NULL){
			if (feof(stdin)){
				return EXIT_SUCCESS;
			}
			else if (ferror(stdin)){
				#if DEBUG
				fprintf(stderr, "Error!!! End of file not reached\n");
				#endif
				fclose(stdin);
			}
			else{
				status = 1;
			}
		}
		else{ 
			args = parseLine(line); /* parse elements of line */
			status = parseArgs(args); /*determines exit status*/

			/* Redirection for built in commands here*/
			if (redirectIn){
				freopen("/dev/tty",  "r", stdin);
				redirectIn = false;
			}
			if (redirectOut){
				freopen("/dev/tty",  "w", stdout);
				redirectOut = false;
			}
			free(args);
		}
		free(line);
	
	} while (status);

	return EXIT_SUCCESS;
}

/* readLine
 * returns string of input */
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

/* parseLine
 * returns an array of arguments parsed from line */
char **parseLine(char *line){
	char *token;
	char *tempLine;
	int i = 0, j = 0, k = 0;
	char **args =  malloc(BUF_SIZE * sizeof(char*));

	count = 0; 

	/* Checking for '<' or '>' redirection symbols and 
	 * obtaining names of files for input/output redirection 
	 * j is the end of the file name
	 * k counts the white spaces
	 * i is the beginning of the '<' or '>' symbol*/
	for (i = 0; i < strlen(line); i++){
		if (line[i] == '<'){
			redirectIn = true;
			j = i;
			j++;
			while (line[j] == ' '){
				j++;
				k++;
			}	
			while(line[j] != ' '&& line[j] != '\n' && line[j]!='>'){
				j++;
			}
			fileIn = malloc (j-k-i+2);
			snprintf(fileIn, j-k-i, "%s", &line[i+k+1]);
		}
		k = 0;

		if (line[i] == '>'){
			redirectOut = true;
			j = i;
			j++;
			while (line[j] == ' '){
				j++;
				k++;
			}	
			while(line[j] != ' ' && line[j] != '\n'&& line[j]!='<'){
				j++;
			}

			fileOut = malloc (j-k-i+2);
			snprintf(fileOut, j-k-i, "%s", &line[i+k+1]);
		}
	}

	/* Remove '<' and '>' elements from command line */
	for (j = 0; j < strlen(line); j++){
		if (line[j] == '<' || line[j] == '>' ){
			tempLine =  malloc(BUF_SIZE);
			memcpy(tempLine, line, BUF_SIZE);
			snprintf(line, j+1, "%s", tempLine);
			break;
		}
	}

	/* Parse Line by white space delimiter*/
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

/* parseArgs 
 * checks to see if command is built in functions should be called
 * if not, passes command and arguments to runExternalCommands */
int parseArgs(char **args){

	int i = 0;
	if (args[0] == NULL){
		fprintf(stderr,"error, empty command entered\n");
		return 1;
	}
	
	for (i = 0; i < numCustom; i++){
		if(strcmp(args[0], customNames[i]) == 0){
			/* input redirection for builtin function */
			if (redirectIn){
				freopen(fileIn, "r", stdin);
			}

			/* output redirection for builtin function*/
			if (redirectOut){
				freopen(fileOut, "w", stdout);
			}
			return (*customFunc[i]) (args); /*call to build in function */
		}
	}
	return runExternalCommands(args);
}

/* function: runExternalCommands
 * runs listf, calc or unix functions 
 * parameters: args (array of all args)
 * returns 1 on success
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
		/* Child process */

		/* input redirection */
		if (redirectIn){
			freopen(fileIn, "r", stdin);
		}

		/* output redirection */
		if (redirectOut){
			freopen(fileOut, "w", stdout);
		}

		/* check for external known commands 
		 	and append appropriate file path */
		if (strcmp(command, "calc") == 0){
			command = strcat(currentWD, "/calc");
			
		}
		if (strcmp (command, "listf")  == 0){
			command = strcat(currentWD, "/listf");
		}

		/* execvp command to run command*/
		if (execvp(command, args) == -1){
		 	perror("Error: Command Not Found");
		 	#if DEBUG
		 		fprintf(stderr, "command: %s\n", command);
		 	#endif
		}
		exit(EXIT_FAILURE);
	}
	else{
		/* Parent Process */
		waitpid(pid, &status, 0);
		
		if (redirectIn){
			freopen("/dev/tty",  "r", stdin);
			redirectIn = false;
		}
		if (redirectOut){
			freopen("/dev/tty",  "w", stdout);
			redirectOut = false;
		}

		return 1;
	}

	return 1;
}

/* function: k_pwd
 * parameters: args (array of all args)
 * prints working directory
 * returns 1 on success */
int k_pwd (char **args){
	char current[1024];
	if (getcwd(current, sizeof(current)) != NULL){
   	#if DEBUG
       fprintf(stdout, "%s ", current);
   	#endif
   }
   	else{
       perror("getcwd() error");
	}
	return 1;
}



/* function: k_cwd
 * parameters: args (array of all args)
 * changes directory to given argument 
 * returns 1 on success */
int k_cwd(char **args){
	char current[1024];
	char home[1024];

	char *defaultChar = "~";
	strcpy(home, "/home/");
	strcat(home, user);


	if (args[1] == NULL){
		#if DEBUG
			fprintf(stdout, "No directory given. Changing to default.");
		#endif
		/* Changes to /home/$USER as default directory*/
		if (chdir(home) !=0){
			#if DEBUG
				fprintf(stdout, "Error with cwd ");
			#endif
		}
	}
	else if (strcmp(args[1], defaultChar) == 0){
		/* Support for cd ~ (which changes to /home/$USER) like UNIX cd */
		if (chdir(home) !=0){
			#if DEBUG
				fprintf(stdout, "Error with cwd ");
			#endif
		}
	}
	else {
		/* Change to given directory */
		if (chdir(args[1]) !=0){
			#if DEBUG
				fprintf(stdout, "No such directory: %s\n", args[1]);
			#endif
			return 1;
		}
	}
	/* Print changed directory */
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

/* function: k_exit
 * parameters: args (array of all args)
 * returns status of 0 to go back to main */
int k_exit(char **args){
	return 0; /* on exit */
}

	