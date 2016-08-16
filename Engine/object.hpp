#pragma once

#include "drawable.hpp"
#include "loader.hpp"

class Object : public Drawable{
public:
  Object(string name, vec3 pos);
  virtual void draw(mat4* MVP);
  vector<Triangle> getTriangles();

private:
  int texture;
  vector<Triangle> triangles;
  GLuint elementsVBO, verticesVBO, texCoordsVBO;

  vector<float> *vertices;
  vector<GLuint> *elements;
  vector<float> *texCoords;

  void addTriangle(vector<GLuint> &index, vector<float> &v, int i);
};

