#pragma once

#include <vector>
#include <glm/glm.hpp>

using namespace std;
using namespace glm;

class Triangle{
public:
	Triangle(vec3 p1, vec3 p2, vec3 p3){
		this->p1 = p1;
		this->p2 = p2;
		this->p3 = p3;
	}
	vec3 p1, p2, p3;
};

template<class T>
GLuint createVBO(vector<T> data){
  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(T), &data[0], GL_STATIC_DRAW);

  return vbo;
}

typedef struct {
	GLint v_index[3];
	GLint t_index[3];
	GLint n_index[3];
} Face;
