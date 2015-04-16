/* heap352.c
* Main memory manager implementing three algorithms to do the functions malloc and free. 
* performs memory allocation and frees memory as needed. Maintains a freelist.
* Compares performance of the three algorithms using system time comparisons.
*/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#ifndef DEBUG
#define DEBUG 1
#endif

#define ALIGNVAL 8 //sizeof(long)
/* Returns appropriate size given a desired size so that it is aligned in 8 byte units*/
#define ALIGN(size) (((size) + (ALIGNVAL-1)) & ~ (ALIGNVAL-1))


void *malloc352 (int size){
	void *cats = (char*) "h";

	#if DEBUG
	fprintf(stdout,"We are having fun!\n");
	#endif

	//First Fit Method


	return cats;
}


void free352 (void *ptr){
	#if DEBUG
	fprintf(stdout,"YAYYYYYYYYYY\n");
	#endif
}
