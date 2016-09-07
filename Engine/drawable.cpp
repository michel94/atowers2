#include "drawable.hpp"

void Drawable::drawTexturedPolygons(GLuint type, GLuint elementsVBO, GLuint verticesVBO, int nVertices, GLuint texId, GLuint texCoordsVBO){
  glBindBuffer(GL_ARRAY_BUFFER, verticesVBO);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, 0);
  
  glBindTexture(GL_TEXTURE_2D, texId);
  glEnableClientState (GL_TEXTURE_COORD_ARRAY_EXT);
  glBindBuffer(GL_ARRAY_BUFFER, texCoordsVBO);
  glTexCoordPointer(2, GL_FLOAT, 0, 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementsVBO);
  
  glDrawElements(type, nVertices, GL_UNSIGNED_INT, 0);
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY_EXT); 
  
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  
}

void Drawable::drawTexturedQuads(GLuint elementsVBO, GLuint verticesVBO, int nVertices, GLuint texId, GLuint texCoordsVBO){
  drawTexturedPolygons(GL_QUADS, elementsVBO, verticesVBO, nVertices, texId, texCoordsVBO);
}

void Drawable::drawTexturedTriangles(GLuint elementsVBO, GLuint verticesVBO, int nVertices, GLuint texId, GLuint texCoordsVBO){
  drawTexturedPolygons(GL_TRIANGLES, elementsVBO, verticesVBO, nVertices, texId, texCoordsVBO);
}

vec3& Drawable::getPosition() {
  return pos;
}

bool Drawable::isClickable(){
  return clickable;
}

void Drawable::setClickable(bool clickable){
  this->clickable = clickable;
}

Properties* Drawable::getProperties(){
  return &properties;
}

string Drawable::getShader(){
  return SHADER;
}
void Drawable::setShader(string shader){
  SHADER = shader;
}