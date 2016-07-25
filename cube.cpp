#include "cube.hpp"

class Drawable;

void Cube::drawSquare(bool flip, int texture){
	float b = 0.0f, t = 1.0f;
	  if(flip)
	    swap(b, t);
	vector<float> texCoords = {b,b, t,b, t,t, b,t};
	vector<float> vertices = { 0.0f, 0.0f, 0,
							            1.0f, 0.0f, 0,
                          1.0f, 1.0f, 0,
                          0.0f, 1.0f, 0};
	Drawable::drawTexturedQuads(vertices, texture, texCoords);
}

void Cube::draw(){
	drawSquare(false, topTexture);
  glBindTexture(GL_TEXTURE_2D, sideTexture);
  glPushMatrix();
    glRotatef(270, 1, 0, 0);
    drawSquare(true, sideTexture);
    glRotatef(270, 0, 1, 0);
    drawSquare(true, sideTexture);
  glPopMatrix();
  glPushMatrix();
    glTranslatef(1, 1, 0);
    glRotatef(180, 0, 0, 1);
    glRotatef(270, 1, 0, 0);
    drawSquare(true, sideTexture);
    glRotatef(270, 0, 1, 0);
    drawSquare(true, sideTexture);
  glPopMatrix();
}

Cube::Cube(int topTexture, int sideTexture){
  this->topTexture = topTexture;
  this->sideTexture = sideTexture;
}