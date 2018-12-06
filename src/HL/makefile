CFLAGS = -g
BIN = ./
CC = gcc

dla:	dla.c maps.o cx.o
	if(test ! -x ${BIN}) then (mkdir ${BIN}) fi
	gcc -O3 dla.c maps.o cx.o -lm -o ${BIN}/ClusterHL
