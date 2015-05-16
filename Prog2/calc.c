/*
	calc.c
	Author: Kathryn McClintic
	Last modified: May 4th, 2015
	Purpose: calculator file 
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include "calc.h"


/* TO DO:
do the whole ./calc thing
get ctrl d working
*/
int main(){
	#if DEBUG
	fprintf(stdout,"Welcome to the coolest calculator on earth\n");
	#endif
	int num1 = 0;
	char op = '+';
	int num2 = 0;



	while (scanf("%d %c%d", &num1, &op, &num2) != EOF){
		calculate (num1, op, num2);
	}
	if (feof(stdin)){
		#if DEBUG
		/*End of File reached */
		fprintf(stderr, "Closing calculator. Goodbye!\n");
		#endif
		fclose(stdin);
	}
	else{
		#if DEBUG
		fprintf(stderr, "Error! End of file not reached\n");
		#endif
		fclose(stdin);
	}
	return 0; 

}
int calculate (int num1, char op, int num2){
	int answer; 

	switch(op)
	{
		case '+':
			answer = num1 + num2;
			#if DEBUG
			fprintf(stdout, "%d %c %d = %d\n",num1 ,op , num2, answer);
			#endif
			break;
		case '-':
			answer = num1 - num2;
			#if DEBUG
			fprintf(stdout, "%d %c %d = %d\n",num1 ,op , num2, answer);
			#endif
			break;
		case '*': 
			answer = num1 * num2;
			#if DEBUG
			fprintf(stdout, "%d %c %d = %d\n",num1 ,op , num2, answer);
			#endif
			break;
		case '/':
		if (num2 == 0){
			#if DEBUG
			fprintf(stderr, "Cannot divide by 0!\n");
			#endif
			break;
		}
			answer = num1 / num2;
			#if DEBUG
			fprintf(stdout, "%d %c %d = %d\n",num1 ,op , num2, answer);
			#endif
			break;
		default :
			#if DEBUG
			fprintf(stderr, "No legal operation requested\n");
			#endif
	}
	return 0; // success
}