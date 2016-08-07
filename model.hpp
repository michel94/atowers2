#include "opengl.h"
#include "utils.hpp"
#include <vector>

using namespace std;

class Model{
public:
	vector<float> vertices;
	vector<GLuint> elements;
	vector<float> texCoords;
	int texture;
	GLuint elementsVBO, verticesVBO, texCoordsVBO;
	Model(vector<float> vertices, vector<GLuint> elements, vector<float> texCoords){
		this->vertices = vertices;
		this->elements = elements;
		this->texCoords = texCoords;
		elementsVBO = createVBO(elements);
		verticesVBO = createVBO(vertices);
		texCoordsVBO = createVBO(texCoords);
	}
};

