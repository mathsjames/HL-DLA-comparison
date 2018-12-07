CFLAGS = -g
BIN = ./
CC = gcc

clusterHL:	clusterHL.c maps.o cx.o
	if(test ! -x ${BIN}) then (mkdir ${BIN}) fi
	gcc -O3 clusterHL.c maps.o cx.o -lm -o ${BIN}/ClusterHL
