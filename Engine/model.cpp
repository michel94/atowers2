#include "model.hpp"

Model::Model(vector<float> vertices, vector<GLuint> elements, vector<float> texCoords){
	this->vertices = vertices;
	this->elements = elements;
	this->texCoords = texCoords;
	elementsVBO = createVBO(elements);
	verticesVBO = createVBO(vertices);
	texCoordsVBO = createVBO(texCoords);
}