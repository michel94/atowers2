#pragma once

#include "drawable.hpp"
#include "loader.hpp"

class Cube : public Drawable{
public:
  Cube(int, vec3);
  virtual void draw(float elapsed, ShaderData& shader, mat4* MVP); // TODO: check this virtual
  virtual void pureDraw(ShaderData& shader, mat4* MVP);

private:
  int texture;
  vector<Triangle> triangles;
  GLuint elementsVBO, verticesVBO, texCoordsVBO;

  vector<float> *vertices;
  vector<GLuint> *elements;
  vector<float> *texCoords;

  void addTriangle(vector<GLuint> &index, vector<float> &v, int i);
};

