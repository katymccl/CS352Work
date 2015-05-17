/*
	Assg2.h
	Author: Kathryn McClintic
	Last modified: May 4th, 2015
	Purpose: header file for Assg2.c
*/


/* Debugging Options */
#ifndef DEBUG
#define DEBUG 1
#endif

#define BUF_SIZE 1024
#define TOKEN_DELIM " "
#define numCustom 3

int (*customFunc[3]) (char **);
int k_exit(char** args);
int k_cwd(char** args);
int k_pwd(char** args);

int runExternalCommands(char **args);
int main();
char **parseLine(char *line);
int parseArgs(char **args);
char *readLine();

int listf();
int calc();

static char currentWD[1024];
static char *user;

typedef enum {false, true} bool;
