/*  listf.h

    Header file for calc program
    CSCI 352, Assignment 2
    Spring, 2015

    Kathryn McClintic May 2015

*/

/* Debugging Options */
#ifndef DEBUG
#define DEBUG 1
#endif
#define BUF_SIZE 1024

typedef enum {false, true} bool;

static char currentWD[1024];

#define MAXDIR 25

int main();
int compareDir ( const struct dirent **a, const struct dirent **b);
int entriesFilter(const struct dirent *entry);
char *getPermissions(mode_t mode);