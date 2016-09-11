#pragma once

#include "drawable.hpp"
#include "loader.hpp"

class Object : public Drawable{
public:
  Object(string name, vec3 pos);
  
  virtual void draw(float elapsed, ShaderData& shader, mat4* MVP);
  vector<Triangle> getTriangles();
  vec2 getSize();
  virtual void setAngle(float);
  virtual float getAngle();

private:
  int texture;
  vector<Triangle> triangles;
  GLuint elementsVBO, verticesVBO, texCoordsVBO;

  vector<float> *vertices;
  vector<GLuint> *elements;
  vector<float> *texCoords;

  void addTriangle(vector<GLuint> &index, vector<float> &v, int i);
protected:
  vec2 size = vec2(1.0, 1.0);
  float angle = 0.0f;
};

