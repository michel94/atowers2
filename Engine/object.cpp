#include "object.hpp"
#include <glm/gtc/matrix_transform.hpp>

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

  setClickable(true);
}

void Object::draw(float elapsed, ShaderData& shader, mat4* MVP){
  Object::pureDraw(shader, MVP);
}

void Object::pureDraw(ShaderData& shader, mat4* MVP){
  mat4 centerT = translate(mat4(), vec3(-0.5, -0.5, 0));
  mat4 rot = rotate(mat4(), angle, vec3(0,0,1));
  mat4 decenterT = translate(mat4(), vec3(0.5, 0.5, 0));
  mat4 tr = translate(mat4(), pos);
  mat4 localMVP = *MVP * tr * decenterT * rot * centerT;
  glUniformMatrix4fv(shader["MVP"], 1, GL_FALSE, &localMVP[0][0]);
  Drawable::drawTexturedTriangles(elementsVBO, verticesVBO, elements->size(), texture, texCoordsVBO);
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

vec2 Object::getSize(){
  return size;
}

void Object::setAngle(float a){
  angle = a;
}

float Object::getAngle(){
  return angle;
}
