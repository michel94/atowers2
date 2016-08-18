
#include <iostream>
#include "game.hpp"
#include "mapgenerator.hpp"
#include "button.hpp"

#define SCREEN_SIZE_CUT 1.2f

using namespace std;
using namespace glm;

const int SCREEN_WIDTH = 1200, SCREEN_HEIGHT = 675;


void Game::loadScene(){
  int mapHeight = 30, mapWidth = 30;
  engine->loadMap(MapGenerator::generateMap(2, mapHeight, mapWidth), mapHeight, mapWidth);
  
  for(int i=0; i<10; i++){
    Object* castle = new Castle(vec3(rand()%mapWidth, rand()%mapHeight, 0.0f));
    engine->addObject3d(castle);
    engine->makeClickable(castle, true);
  }

  vec2 size = vec2(64, 64);
  Button* farmButton = new Button("castle", vec2(SCREEN_WIDTH, SCREEN_HEIGHT) - size, size);
  engine->addObject2d(farmButton);
  engine->makeClickable(farmButton, true);

}

Game::Game(){
  engine = new Engine(this, SCREEN_WIDTH, SCREEN_HEIGHT);
  loadScene();

  engine->run();
}

void Game::onOver(Drawable* obj){
  if(cursor){
    vec3 pos = obj->getPosition();
    vec3& cursorPos = cursor->getPosition();
    cursorPos.x = pos.x;
    cursorPos.y = pos.y;
    cursorPos = vec3(pos.x, pos.y, engine->getTerrainHeight((int)pos.x, (int)pos.y));
  }
}

void Game::onClick(Drawable* obj, int button){
  if(button == GLFW_MOUSE_BUTTON_RIGHT){
    engine->removeObject3d(cursor);
    delete cursor;
    cursor = NULL;
  }
   // TODO: map strings to constructors
  if(cursor){
    vec3 pos = obj->getPosition();
    Object* castle = new Castle(vec3(pos.x, pos.y, 0));
    engine->addObject3d(castle);
    engine->makeClickable(castle, true);
  }
}

void Game::onMenuOver(Drawable2d* obj){
  
}

void Game::onButtonClick(string name){
  if(cursor)
    engine->removeObject3d(cursor);
  if(name == "castle")
    cursor = new Castle(vec3(0, 0, 0));
  //else if(name == "farm")
  //  cursor = new Farm(vec3(0, 0, 0));
  else
    return;

  engine->addObject3d(cursor);

}

void Game::onMenuClick(Drawable2d* obj, int button){
}

int main(int argc, char **argv){
  new Game();
  
	return 0;
}


