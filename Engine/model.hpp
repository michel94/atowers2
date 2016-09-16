#pragma once

#include "opengl.h"
#include <vector>
#include "utils.hpp"

using namespace std;

class Model{
public:
	vector<float> vertices;
	vector<GLuint> elements;
	vector<float> texCoords;
	int texture;
	GLuint elementsVBO, verticesVBO, texCoordsVBO;
	Model(vector<float> vertices, vector<GLuint> elements, vector<float> texCoords);
};

