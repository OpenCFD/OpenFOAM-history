# you need a compiler that does ANSI prototypes
#CC=gcc
#CFLAGS=-O -Wall
CC=gcc
CFLAGS=-O0
#CFLAGS=-n32 -O2 -mips3 
#CFLAGS=-64 -O2 -mips4

OBJS=calloc.o free.o malloc.o morecore.o realloc.o valloc.o

# If you are mixing this malloc with another C library.
alloc.o: $(OBJS)
	ld -r -o alloc.o $(OBJS)
	rm $(OBJS)

# If you are building a C library.
lib.a: $(OBJS)
	ar r lib.a $(OBJS)

clean:
	rm -f alloc.o lib.a $(OBJS)

$(OBJS): malloc.h
