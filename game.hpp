#pragma once

#include <stdio.h>
#include <iostream>
#include <string.h>
#include <glm/glm.hpp>
#include "Engine/engine.hpp"

#ifdef __unix
	#define CONSOLE_FPS 1
#endif

class Building : public Object {
public:
  Building(string name, vec3 pos) : Object(name, pos){
    this->name = name;
  };
  ~Building();
  void onClick(GameLogic* game, int button){
    printf("Clicked on %s\n", name.c_str());
  }
  string getName(){
    return name;
  }
private:
  string name;
};

class Castle : public Object {
public: 
  Castle(vec3 pos) : Object("castle", pos){
  };
  ~Castle();
  void onClick(GameLogic* game, int button){
    printf("Clicked on castle!\n");
  }
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
  Object* cursor = NULL;
};

