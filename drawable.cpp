#include "drawable.hpp"

void Drawable::drawTexturedQuads(vector<float> vertices, GLint texId, vector<float> texCoords){
  glBindTexture(GL_TEXTURE_2D, texId);

  glPushMatrix();
  glTranslatef(pos.x, pos.y, pos.z);
  glBegin(GL_QUADS);
    for(int i=0; i<(signed)vertices.size()/3; i++){
      glTexCoord2f(texCoords[i*2], texCoords[i*2+1]);
      glVertex3f(vertices[i*3], vertices[i*3+1], vertices[i*3+2]);
    }
  glEnd();
  glPopMatrix();
}
