
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
    Building* building = new Building("hut", vec3(rand()%mapWidth, rand()%mapHeight, 0.0f));

    engine->addObject3d(building);
    engine->makeClickable(building, true);
  }

  vector<Button*> buttons;
  vector<string> buildings = {"castle", "farm", "hut", "tower", "rock"};
  
  dimensions["castle"] = vec2(1, 1);
  dimensions["farm"] = vec2(1, 1);
  dimensions["hut"] = vec2(1, 1);
  dimensions["tower"] = vec2(2, 2);
  dimensions["tower"] = vec2(1, 1);

  int gridWidth = 3;
  vec2 size = vec2(64, 64);
  for(int i=0; i<(signed)buildings.size(); i++){
    Button* button = new Button(buildings[i], vec2(980 + (i%gridWidth)*(size.x+8), SCREEN_HEIGHT -30 - size.y - (i/gridWidth)*(size.y+8) ), size);
    engine->addObject2d(button);
    engine->makeClickable(button, true);

  }

}

Game::Game(){
  srand(time(NULL));
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

    if(cursor->over)
      cursor->over->overred = false;

    cursor->over = (Building*) engine->at(pos.x, pos.y);
    if(cursor->over != NULL){
      cursor->over->overred = true;
    }
  }
}

void Game::onClick(Drawable* obj, int button){
  if(button == GLFW_MOUSE_BUTTON_RIGHT){
    engine->removeObject3d(cursor);
    cursor = NULL;
  }
   // TODO: map strings to constructors
  if(cursor && button == GLFW_MOUSE_BUTTON_LEFT){
    vec3 pos = obj->getPosition();
    string name = ((Building*)cursor)->getName();
    Building* building = new Building( name, vec3(pos.x, pos.y, 0));
    engine->addObject3d(building);
    engine->makeClickable(building, true);
  }
}

void Game::onMenuOver(Drawable2d* obj){
}

void Game::onButtonClick(string name){
  if(cursor){
    engine->removeObject3d(cursor);
    if(cursor->over)
      cursor->over->overred = false;
  }
  
  cursor = new Cursor(name, vec3(0, 0, 0), engine);
  if(!cursor)
    return;
  
  engine->addObject3d(cursor);

}

void Game::onMenuClick(Drawable2d* obj, int button){
}

int main(int argc, char **argv){
  new Game();
  
	return 0;
}


