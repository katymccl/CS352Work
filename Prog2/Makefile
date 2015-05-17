# Kathryn McClintic
# Makefile for Command line interpreter 
# May 2015

CC=gcc
CFLAGS=-c -Wall -g 
LFLAGS=-g
SOURCES=*.c *.h
OBJECTS=*.o 
EXES=calc Assg2 listf



all: $(EXES) 

$(EXES): calc.o Assg2.o listf.o
	$(CC) $(LFLAGS) calc.o -o calc
	$(CC) $(LFLAGS) listf.o -o listf
	$(CC) $(LFLAGS) Assg2.o -o Assg2

Assg2.o: Assg2.h Assg2.c calc.h listf.h
	$(CC) $(CFLAGS) Assg2.c
calc.o: calc.h calc.c
	$(CC) $(CFLAGS) calc.c

listf.o: listf.h listf.c
	$(CC) $(CFLAGS) listf.c

clean: 
	rm -f *.o $(EXES)
#HeapTestEngine.o: heap352.h HeapTestEngine.c
	#$(CC) $(CFLAGS) HeapTestEngine.c

#heap352.o: heap352.c 
	#$(CC) $(CFLAGS) heap352.c

#clean:
	#rm -f *.o $(EXES)

