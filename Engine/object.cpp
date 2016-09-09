#include "object.hpp"
#include <glm/gtc/matrix_transform.hpp>

void Object::draw(float elapsed, ShaderData& shader, mat4* MVP){
  mat4 tr = translate(mat4(), pos);
  mat4 localMVP = *MVP * tr;
  glUniformMatrix4fv(shader["MVP"], 1, GL_FALSE, &localMVP[0][0]);
  glColor4f(1.0, 0.0, 0.0, 1.0);
  Drawable::drawTexturedTriangles(elementsVBO, verticesVBO, elements->size(), texture, texCoordsVBO);
}

vector<Triangle> Object::getTriangles(){
  return triangles;
}

void Object::addTriangle(vector<GLuint> &index, vector<float> &v, int i) {
  int idx1 = 3*index[i],
      idx2 = 3*index[i+1],
      idx3 = 3*index[i+2];
  auto v1 = vec3(v[idx1], v[idx1+1], v[idx1+2]),
       v2 = vec3(v[idx2], v[idx2+1], v[idx2+2]),
       v3 = vec3(v[idx3], v[idx3+1], v[idx3+2]);
  this->triangles.push_back(Triangle(v1, v2, v3));
}

Object::Object(string name, vec3 pos){
  Model* model = Loader::loadModel(name);
  
  this->texture = model->texture;
  this->pos = pos;
  
  elementsVBO  = model->elementsVBO;
  verticesVBO  = model->verticesVBO;
  texCoordsVBO = model->texCoordsVBO;
  elements  = &model->elements;
  vertices  = &model->vertices;
  texCoords = &model->texCoords;

  for(int i=0; i<(signed)elements->size(); i+=3){
    addTriangle(*elements, *vertices, i);
  }
}

vec2 Object::getSize(){
  return size;
}