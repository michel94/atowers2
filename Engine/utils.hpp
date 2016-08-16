#pragma once

#include <vector>
#include <algorithm>
#include <glm/glm.hpp>

using namespace std;
using namespace glm;

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

template<class T>
T remove(vector<T> &v, T value){
	auto it = find(v.begin(), v.end(), value);
	
	if(it == v.end())
		return *(v.end());
	auto element = *it;
	*it = v.back();
	v.pop_back();
	return element;
}
