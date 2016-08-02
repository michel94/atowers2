#include "drawable.hpp"

void Drawable::drawTexturedQuads(GLuint elementsVBO, GLuint verticesVBO, int nVertices, GLuint texId, GLuint texCoordsVBO){  
  glBindBuffer(GL_ARRAY_BUFFER, verticesVBO);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, 0);
  
  glBindTexture(GL_TEXTURE_2D, texId);
  glEnableClientState (GL_TEXTURE_COORD_ARRAY_EXT);
  glBindBuffer(GL_ARRAY_BUFFER, texCoordsVBO);
  glTexCoordPointer(2, GL_FLOAT, 0, 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementsVBO);
  
  glDrawElements(GL_QUADS, nVertices, GL_UNSIGNED_INT, 0);
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY_EXT); 
  
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  
}
