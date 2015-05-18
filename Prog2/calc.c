/*
	calc.c
	Author: Kathryn McClintic
	Last modified: May 17th, 2015
	Purpose: calculator file 
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include "calc.h"

int main(){
	int num1 = 0;
	char op = '+';
	int num2 = 0;

	/*getting input and calls calculate 
	 * Will only work if number, operator, number is entered */
	
	while (scanf("%d %c%d", &num1, &op, &num2) != EOF){
		if (op == '+' || op =='-' || op == '/' || op == '*'){
				calculate (num1, op, num2);
		}
		
	}
	return 0; 
}

/* calculate
 * takes a num1, character operation, and num2
 * performs  operation. */
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
			fprintf(stdout, "Cannot divide by 0!\n");
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
