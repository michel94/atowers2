#pragma once

#include <stdio.h>
#include <iostream>
#include <string.h>
#include <utility>
#include <glm/glm.hpp>
#include "Engine/engine.hpp"

#ifdef __unix
  #define CONSOLE_FPS 1
#endif

class Building : public Object {
public:
  explicit Building(string name, vec3 pos) : Object("buildings/" + name, pos){
    this->name = name;
    setShader("blended");
  };
  ~Building(){;}
  void onClick(GameLogic* game, int button){
    printf("Clicked on %s\n", name.c_str());
  }
  virtual void draw(ShaderData& shader, mat4* MVP){
    if(overred){
      glUniform4fv(shader["colorMix"], 1, &colorMix[0]);
    }else{
      vec4 v = vec4(1.0, 1.0, 1.0, 1.0);
      glUniform4fv(shader["colorMix"], 1, &v[0]);
    }
    Object::draw(shader, MVP);
  }

  string getName(){
    return name;
  }

  bool overred = false;
private:
  vec4 colorMix = vec4(1.5, 0.5, 0.5, 1.0);
  string name;
};

class Cursor : public Building {
public:
  Cursor(string name, vec3 pos, Engine* engine) : Building(name, pos){
    setShader("blended");
    this->engine = engine;
  }
  virtual void draw(ShaderData& shader, mat4* MVP){
    if(over){
      ;//glUniform4fv(shader["colorMix"], 1, &colorMix[0]);
    }else{
      Building::draw(shader, MVP);
    }
  }

  Building* over = NULL;
private:
  bool canBuild = false;
  Engine* engine;
};

class Game : public GameLogic{
public:
  Game();
  void loadScene();
  void onOver(Drawable* obj);
  void onClick(Drawable* obj, int button);

  void onMenuOver(Drawable2d* obj);
  void onMenuClick(Drawable2d* obj, int button);
  void onButtonClick(string name);
private:
  Engine* engine;
  Cursor* cursor = NULL;
  map<string, vec2> dimensions;
};

