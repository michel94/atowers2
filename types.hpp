#ifndef TYPES_H
#define TYPES_H

#ifdef __APPLE__
	#include <OpenGL/gl.h>
#else
	#include <GL/gl.h>
#endif

typedef struct {
	GLfloat x, y, z;
} Point;

typedef struct {
	GLfloat x, y;
} Point2;

typedef struct {
	GLint v_index[3];
	GLint t_index[3];
	GLint n_index[3];
} Face;

#endif
