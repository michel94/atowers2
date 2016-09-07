#include "cube.hpp"
#include <glm/gtc/matrix_transform.hpp>

void Cube::draw(ShaderData& shader, mat4* MVP){
  mat4 tr = translate(mat4(), vec3(pos.x, pos.y, pos.z));
  mat4 localMVP = *MVP * tr;
  glUniformMatrix4fv(shader["MVP"], 1, GL_FALSE, &localMVP[0][0]);
  Drawable::drawTexturedQuads(elementsVBO, verticesVBO, elements->size(), texture, texCoordsVBO);
}

vector<Triangle> Cube::getTriangles(){
  return triangles;
}

void Cube::addTriangle(vector<GLuint> &index, vector<float> &v, int i) {
  int idx1 = 3*index[i],
      idx2 = 3*index[i+1],
      idx3 = 3*index[i+2],
      idx4 = 3*index[i+3];
  auto v1 = vec3(v[idx1], v[idx1+1], v[idx1+2]),
       v2 = vec3(v[idx2], v[idx2+1], v[idx2+2]),
       v3 = vec3(v[idx3], v[idx3+1], v[idx3+2]),
       v4 = vec3(v[idx4], v[idx4+1], v[idx4+2]);
  this->triangles.push_back(Triangle(v1, v2, v3));
  this->triangles.push_back(Triangle(v1, v3, v4));
}

Cube::Cube(int texture, vec3 pos){
  this->texture = texture;
  this->pos = pos;

  Model* model = Loader::loadModel("cube");
  
  elementsVBO  = model->elementsVBO;
  verticesVBO  = model->verticesVBO;
  texCoordsVBO = model->texCoordsVBO;
  elements  = &model->elements;
  vertices  = &model->vertices;
  texCoords = &model->texCoords;

  for(int i=0; i<(signed)elements->size(); i+=4){
    addTriangle(*elements, *vertices, i);
  }
}
