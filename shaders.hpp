#ifndef SHADERS_H
#define SHADERS_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#ifdef __APPLE__
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
	#include <OpenGL/glext.h>
#else
	#include <GL/gl.h>
	#include <GL/glu.h>
	#include <GL/glext.h>
#endif

#include "types.hpp"

using namespace std;

GLuint load_shaders(const char * vertex_file_path, const char * fragment_file_path);

#endif
