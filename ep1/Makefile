CC=gcc
CFLAGS=-Wall -pedantic -g
READLINE=-lreadline

all: ep1

ep1: process.o ep1.o
	${CC} ${CFLAGS} -o ep1 

process.o: process.c process.h

ep1.o: ep1.c

clean: rm ep1 ep1sh *.o