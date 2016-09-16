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
  virtual void pureDraw(ShaderData& shader, mat4* MVP){;}
  void drawTexturedQuads(GLuint elementsVBO, GLuint verticesVBO, int nVertices, GLuint texId, GLuint texCoordsVBO);
  void drawTexturedTriangles(GLuint elementsVBO, GLuint verticesVBO, int nVertices, GLuint texId, GLuint texCoordsVBO);
  void drawTexturedPolygons(GLuint type, GLuint elementsVBO, GLuint verticesVBO, int nVertices, GLuint texId, GLuint texCoordsVBO);
  virtual vec3& getPosition();
  virtual void setPosition(vec3);
  virtual bool isClickable();
  virtual void setClickable(bool);
  virtual string getShader();
  virtual void setShader(string shader);

  virtual void onClick(GameLogic*, int button, int action, int mods) = 0;

  virtual Properties defaultProperties(){
    return {false, false};
  }

  Properties* getProperties(){
    if(!properties){
      properties = new Properties;
      *properties = defaultProperties();
    }
    return properties;
  }

protected:
  vec3 pos = vec3(0,0,0);
  bool clickable = false;

  string SHADER = "textured";
  Properties* properties = NULL;
};

class Drawable2d : public Drawable{
public:
  virtual vec2 getSize() = 0;

  virtual Properties defaultProperties(){
    return {true, false};
  }
  virtual Drawable2d* overredObject(vec2 scale, float mouseX, float mouseY){
    vec2 pos = scale * vec2(getPosition().x, getPosition().y);
    vec2 size = scale * getSize();
    if(contains(pos, size, vec2(mouseX, mouseY)))
      return this;
    return NULL;
  }

protected:
  Properties properties = {true, false};
  string SHADER = "none";

private:
  bool contains(vec2 orig, vec2 size, vec2 point){
    return  point.x >= orig.x && point.x <= orig.x + size.x &&
        point.y >= orig.y && point.y <= orig.y + size.y;
  }
};

class Group2d : public Drawable2d{
public:
  Group2d(vec3 pos) {
    this->pos = pos;
    properties = {true, true};
  }
  Group2d(vec3 pos, Drawable2d* child) {
    this->pos = pos;
    properties = {true, true};
    addChild(child);
  }
  Group2d(vec3 pos, std::vector<Drawable2d*> children) {
    this->pos = pos;
    properties = {true, true};
    for(int i=0; i<(signed)children.size(); i++){
      addChild(children[i]);
    }
  }
  virtual void draw(float elapsed, ShaderData& shader, mat4* MVP){
    if(!shaders){
      cout << "Error! Group2d doesn't have a reference to the ShaderMap\n";
      return;
    }
    mat4 mvp = translate(*MVP, vec3(pos.x, pos.y, 0)); // Don't do this. two different translation methods
    for(int i=0; i<(signed)children.size(); i++){
      ShaderData* shader = (*shaders)[children[i]->getShader()];
      glUseProgram(shader->getProgram());
      children[i]->draw(elapsed, *shader, &mvp);
    }
    
  }
  void setShaders(ShaderMap* shaders){
    this->shaders = shaders;
  }

  void addObject(Drawable2d* obj) {
    addChild(obj);
  }
  virtual vec2 getSize(){
    return size;
  }
  virtual Properties defaultProperties(){
    return {true, true};
  }
  void onClick(GameLogic*, int button, int action, int mods){

  }
  virtual Drawable2d* overredObject(vec2 scale, float x, float y){
    //cout << getPosition().x << ", " << getPosition().y << "; size: " << getSize().x << ", " << getSize().y << "; " << x << ", " << y << endl;
    if(!Drawable2d::overredObject(scale, x, y))
      return NULL;
    vec2 pos = vec2(x, y) - scale * vec2(getPosition().x, getPosition().y);
    vec2 size = scale * getSize();
    Drawable2d* over = NULL;
    for(int i=0; i<(signed)children.size(); i++){
      Drawable2d* obj = children[i]->overredObject(scale, pos.x, pos.y);
      if(obj)
        over = obj;
    }
    return over;
  }
private:
  vector<Drawable2d*> children;
  vec2 size = vec2(0,0);
  void addChild(Drawable2d* child){
    vec3 v3 = child->getPosition();
    vec2 s = vec2(v3.x, v3.y) + child->getSize();
    size = vec2(std::max(size.x, s.x), std::max(size.y, s.y));
    children.push_back(child);
  }

  ShaderMap* shaders = NULL;
protected:
};
