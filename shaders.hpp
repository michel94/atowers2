#ifndef SHADERS_H
#define SHADERS_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "opengl.h"

using namespace std;

GLuint loadShader(const char * vertex_file_path, const char * fragment_file_path);

#endif
