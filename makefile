CC=gcc
CFLAGS=-ansi -Wall -std=c99
GLFLAGS=-lGL -lGLU -lglut
SRC=main.c
all:
	$(CC) $(CFLAGS) $(SRC) $(GLFLAGS) 
