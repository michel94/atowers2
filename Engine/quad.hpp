#pragma once

#include "drawable.hpp"
#include <iostream>
#include "loader.hpp"

class Quad : public Drawable2d{
public:
  Quad(vec2 pos){
    setShader("textured");
    this->pos = vec3(pos.x, pos.y, 0);
    model = Loader::loadQuad();
  }
  Quad(int texture, vec2 pos, vec2 size){
    setShader("textured");
    this->texture = texture;
    this->pos = vec3(pos.x, pos.y, 0);
    this->size = size;
    model = Loader::loadQuad();
  }
  void draw(float elapsed, ShaderData& shader, mat4* MVP){
    glEnable (GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    mat4 localMVP = translate(*MVP, pos);
    localMVP = scale(localMVP, vec3(size.x, size.y, 0) );
    glUniformMatrix4fv(shader["MVP"], 1, GL_FALSE, &localMVP[0][0]);
    Drawable::drawTexturedQuads(model->elementsVBO, model->verticesVBO, model->elements.size(), texture, model->texCoordsVBO);
  }
  virtual vec2 getSize(){
    return size;
  }
  virtual void setSize(vec2 size){
    this->size = size;
  }

  void onClick(GameLogic*, int button, int action, int mods){}
private:
protected:
  int texture;
  vec2 size;
  Model* model = NULL;
  //GLuint elementsVBO, verticesVBO, texCoordsVBO;
};
