#pragma once

#include <stdio.h>
#include <iostream>
#include <string.h>
#include <utility>
#include <glm/glm.hpp>
#include "Engine/engine.hpp"
#include "building.hpp"

#ifdef __unix
  #define CONSOLE_FPS 1
#endif

class Cursor : public Building {
public:
  Cursor(string name, vec3 pos, Engine* engine) : Building(name, pos){
    setShader("blended");
    this->engine = engine;
  }
  virtual void draw(float elapsed, ShaderData& shader, mat4* MVP){
    if(!over)
      Building::draw(elapsed, shader, MVP);
    
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
  Resources myResources;
  Engine* engine;
  Cursor* cursor = NULL;
  map<string, vec2> dimensions;
};

