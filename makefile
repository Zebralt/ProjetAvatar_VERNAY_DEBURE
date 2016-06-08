#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

LIBS = -lGL -lGLU -lglut -lm

avatar:
	gcc -o $@ main.c $(LIBS)
