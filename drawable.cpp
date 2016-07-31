#include "drawable.hpp"

void Drawable::drawTexturedQuads(GLuint verticesVBO, int nVertices, GLuint texId, GLuint texCoordsVBO){
  glPushMatrix();
    glTranslatef(pos.x, pos.y, pos.z);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState (GL_TEXTURE_COORD_ARRAY_EXT);
    
    glBindTexture(GL_TEXTURE_2D, texId);
    glBindBuffer(GL_ARRAY_BUFFER, texCoordsVBO);
    glTexCoordPointer(2, GL_FLOAT, 0, 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, verticesVBO);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    
    glDrawArrays(GL_QUADS, 0, nVertices);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY_EXT); 
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  glPopMatrix();
}

GLuint Drawable::createVBO(vector<float> data){
  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(float), &data[0], GL_STATIC_DRAW);

  return vbo;
}