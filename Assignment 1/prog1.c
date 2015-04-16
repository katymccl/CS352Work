/* Main testing file. 
* Calls heap_test().
*
* Will be the main executable file used to test malloc352 and free352.
* 
*/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "HeapTestEngine.h"

int main(){
	fprintf(stdout, "Running heap test\n");
	heap_test();
	return 0;
}
