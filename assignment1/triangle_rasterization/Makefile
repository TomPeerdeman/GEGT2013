CC=gcc

WARNING_FLAGS=-Wall -Wextra -Werror-implicit-function-declaration -Wshadow -Wstrict-prototypes -pedantic-errors
CFLAGS=-g -O2 -std=c99 $(WARNING_FLAGS)
LDFLAGS=-g -lm -lGL -lglut -lGLU

.c.o:
	$(CC) -c $(CFLAGS) $<

all: trirast
trirast: main.o trirast.o
	$(CC) main.o trirast.o -o trirast $(LDFLAGS)

clean:
	rm -f *.o
	rm -f trirast

trirast.o   : types.h trirast.c
types.o     : types.h
main.o      : triangles.h trirast.h types.h main.c
triangles.o : triangles.h triangles.h
trirast.o   : types.h trirast.h
