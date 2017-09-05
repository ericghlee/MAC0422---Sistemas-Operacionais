CC=gcc
CFLAGS=-Wall -pedantic -g

all: ep1

scheduler: process.c file_utils.c
	${CC} ${CFLAGS} -o scheduler process.o file_utils.o ep1.o


# ep1.o: ep1.c

# clean: rm ep1 ep1sh *.o