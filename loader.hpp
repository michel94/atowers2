#pragma once

#include <GL/glew.h>
#include <map>
#include "model.hpp"

using namespace std;

class Loader{
public:
	static int loadPng(const char* );
	static Model* loadModel(string name);
	static Model* loadObj(string path);
	static Model loadGroup(string group_name);

};
