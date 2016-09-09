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
  Cursor(string name, vec3 pos, Resources* res) : Building(name, pos){
    setShader("blended");
    this->res = res;
    //building = BuildingFactory::newBuildingByName()
    //myResources - building->getCost()
  }
  virtual void draw(float elapsed, ShaderData& shader, mat4* MVP){
    if(!over){
      if(res->enoughResources( BuildingFactory::getCost(name) )){
        vec4 v = vec4(1.0, 1.0, 1.0, 1.0);
        glUniform4fv(shader["colorMix"], 1, &v[0]);
      }else{
        glUniform4fv(shader["colorMix"], 1, &colorMix[0]);
      }
      Object::draw(elapsed, shader, MVP);
    }
  }

  Building* over = NULL;
private:
  vec4 colorMix = vec4(1.5, 1.5, 0.5, 1.0);
  bool canBuild = false;
  Building* building;
  Resources* res;
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

  Resources* getResources();
private:
  Resources myResources;
  Engine* engine;
  Cursor* cursor = NULL;
  map<string, vec2> dimensions;
};

