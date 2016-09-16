#pragma once

#include <GL/glew.h>
#include <map>
#include <vector>
#include <glm/glm.hpp>
#include "utils.hpp"
#include "model.hpp"

using namespace std;
using namespace glm;

class Loader{
public:
	static int loadPng(string);
	static Model* loadModel(string name);
  static Model* loadQuad();
	static Model* loadObj(string path);
	static Model loadGroup(string group_name);

};
