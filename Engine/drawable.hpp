#pragma once

#include "opengl.h"
#include "gamelogic.hpp"
#include "shaders.hpp"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;

struct Properties{
  bool is2d;
  bool clickable;
};

class Triangle{
public:
  Triangle(vec3 p1, vec3 p2, vec3 p3){
    this->p1 = p1;
    this->p2 = p2;
    this->p3 = p3;
  }
  vec3 p1, p2, p3;
};

class Drawable{
public:
  virtual void draw(float elapsed, ShaderData& shader, mat4* MVP) = 0;
  void drawTexturedQuads(GLuint elementsVBO, GLuint verticesVBO, int nVertices, GLuint texId, GLuint texCoordsVBO);
  void drawTexturedTriangles(GLuint elementsVBO, GLuint verticesVBO, int nVertices, GLuint texId, GLuint texCoordsVBO);
  void drawTexturedPolygons(GLuint type, GLuint elementsVBO, GLuint verticesVBO, int nVertices, GLuint texId, GLuint texCoordsVBO);
  virtual vec3& getPosition();
  virtual void setPosition(vec3);
  bool isClickable();
  void setClickable(bool);
  string getShader();
  void setShader(string shader);

  virtual vector<Triangle> getTriangles(){return vector<Triangle>();}
  virtual void onClick(GameLogic*, int button, int action, int mods) = 0;

  virtual Properties* getProperties();
protected:
  vec3 pos = vec3(0,0,0);
  bool clickable = false;

  Properties properties = {false};
  string SHADER = "textured";
};

class Drawable2d : public Drawable{
public:
  virtual vec2 getSize() = 0;

  Properties* getProperties(){
    return &properties;
  }
protected:
  Properties properties = {true};
};

class Group2d : public Drawable2d{
public:
  Group2d(vec3 pos) {
    this->pos = pos;
  }
  Group2d(vec3 pos, Drawable2d* child) {
    this->pos = pos;
    addChild(child);
    
  }
  Group2d(vec3 pos, std::vector<Drawable2d*> children) {
    this->pos = pos;
    for(int i=0; i<(signed)children.size(); i++){
      addChild(children[i]);
    }
  }
  virtual void draw(float elapsed, ShaderData& shader, mat4* MVP){
    glPushMatrix();
      glTranslatef(pos.x, pos.y, 0);
      mat4 mvp = translate(*MVP, vec3(pos.x, pos.y, 0)); // Don't do this. two diferent translations methods
      for(int i=0; i<(signed)children.size(); i++)
        children[i]->draw(elapsed, shader, &mvp);
    
    glPopMatrix();
  }
  void addObject(Drawable2d* obj) {
    addChild(obj);
  }
  virtual vec2 getSize(){
    return size;
  }
  void onClick(GameLogic*, int button, int action, int mods){
    //TODO: propagate to children
  }
private:
  vector<Drawable2d*> children;
  vec2 size = vec2(0,0);
  void addChild(Drawable2d* child){
    vec3 v3 = child->getPosition();
    vec2 s = vec2(v3.x, v3.y) + child->getSize();
    size = vec2(std::max(size.x, s.x), std::max(size.x, size.y));
    children.push_back(child);
  }
};
