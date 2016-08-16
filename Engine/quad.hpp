#pragma once

#include "drawable.hpp"
#include <iostream>

class Quad : public Drawable2d{
public:
  Quad(int texture, vec2 pos, vec2 size){
    this->texture = texture;
    this->pos = vec3(pos.x, pos.y, 0);
    this->size = size;
  }
  void draw(mat4* MVP){
    glEnable (GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
      glTexCoord2f(0.0f, 0.0f); glVertex2f(pos.x, pos.y);
      glTexCoord2f(1.0f, 0.0f); glVertex2f(pos.x+size.x, pos.y);
      glTexCoord2f(1.0f, 1.0f); glVertex2f(pos.x+size.x, pos.y+size.y);
      glTexCoord2f(0.0f, 1.0f); glVertex2f(pos.x, pos.y+size.y);
    glEnd();
  }
  vec2 getSize(){
    return size;
  }
private:
  int texture;
protected:
  vec2 size;
  //GLuint elementsVBO, verticesVBO, texCoordsVBO;
};
