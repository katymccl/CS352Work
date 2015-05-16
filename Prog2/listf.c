/*
	listf.c
	Author: Kathryn McClintic
	Last modified: May 11th, 2015
	Purpose: implements  a custom ls unix command with additional features
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include "listf.h"

int main(int argc, char **argv){



	#if DEBUG
	fprintf(stdout,"Welcome to the most awesome list thingy in the entire universe.\n");
	#endif
	 