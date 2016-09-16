#include "cube.hpp"
#include <glm/gtc/matrix_transform.hpp>

void Cube::draw(float elapsed, ShaderData& shader, mat4* MVP){
  mat4 tr = translate(mat4(), vec3(pos.x, pos.y, pos.z));
  mat4 localMVP = *MVP * tr;
  glUniformMatrix4fv(shader["MVP"], 1, GL_FALSE, &localMVP[0][0]);
  Drawable::drawTexturedQuads(elementsVBO, verticesVBO, elements->size(), texture, texCoordsVBO);
}

void Cube::pureDraw(ShaderData& shader, mat4* MVP){
  draw(0, shader, MVP);
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

}
