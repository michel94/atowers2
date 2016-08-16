#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include "opengl.h"

using namespace std;

GLuint loadShader(const string vertex_file_path, const string fragment_file_path);
GLuint loadShader(const string vertex_file_path, const string fragment_file_path, map<string, GLuint> &ids, vector<string> vars);

