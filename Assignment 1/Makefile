CC=gcc
CFLAGS=-c -Wall -g
LFLAGS=-g
SOURCES=*.c *.h
OBJECTS=*.o 
EXES=prog1

# Alt version#
#all: $(SOURCES) $(OBJECTS)
#	$(CC)  $(LFLAGS) $(OBJECTS) -o $(EXES)
#%.o: %.c %.h
	#$(CC) $(CFLAGS) $< -o $@
#clean:
	#rm *.o $(EXES)

all: $(EXES) 

$(EXES): HeapTestEngine.o heap352.o prog1.o
	$(CC) $(LFLAGS) HeapTestEngine.o  heap352.o prog1.o -o prog1

prog1.o: prog1.c HeapTestEngine.h
	$(CC) $(CFLAGS) prog1.c
HeapTestEngine.o: heap352.h HeapTestEngine.c
	$(CC) $(CFLAGS) HeapTestEngine.c

heap352.o: heap352.c 
	$(CC) $(CFLAGS) heap352.c

clean:
	rm -f *.o $(EXES)

