#ifndef SHADERS_H
#define SHADERS_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include "opengl.h"

using namespace std;

GLuint loadShader(const char * vertex_file_path, const char * fragment_file_path);
GLuint loadShader(const char * vertex_file_path, const char * fragment_file_path, map<string, GLuint> &ids, vector<string> vars);

#endif
