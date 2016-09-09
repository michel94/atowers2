
#include <iostream>
#include "game.hpp"
#include "mapgenerator.hpp"
#include "button.hpp"
#include "Engine/textbox.hpp"

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

  myResources = Resources(10, 27, 20, 20, 10);

  vector<Button*> buttons;
  vector<string> buildings = {"castle", "farm", "hut", "tower", "rock"};
  vector<string> resourceNames = Resources::getResourceList();
  
  dimensions["castle"] = vec2(1, 1);
  dimensions["farm"] = vec2(1, 1);
  dimensions["hut"] = vec2(1, 1);
  dimensions["tower"] = vec2(2, 2);
  dimensions["rock"] = vec2(1, 1);

  int w = 3;
  vec2 size = vec2(64, 64);
  for(int i=0; i<(signed)buildings.size(); i++){
    vec2 pos = vec2(980 + (i%w)*(size.x+8), SCREEN_HEIGHT -200 - size.y - (i/w)*(size.y+8) );
    Button* button = new Button(buildings[i], pos, size);
    engine->addObject2d(button);
    engine->makeClickable(button, true);
    
  }

  int gridWidth = 5;
  vec2 size2 = vec2(32, 32);
  vector<Textbox*> textboxes; textboxes.resize((signed)resourceNames.size());
  for(int i=0; i < (signed) resourceNames.size(); i++){
    vec2 pos = vec2(980 + (i%gridWidth)*(size2.x+8), SCREEN_HEIGHT -10 - size2.y - (i/gridWidth)*(size2.y+8));
    Image* image = new Image(resourceNames[i], pos, size2);
    engine->addObject2d(image);
    engine->makeClickable(image, true);
    
    pos += vec2(7, -13);
    Textbox* textbox = new Textbox(pos, 120, "15");
    engine->addObject2d(textbox);
    textboxes[i] = textbox;

  }
  myResources.setTextboxes(textboxes);

}

Resources* Game::getResources(){
  return &myResources;
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
  if(cursor){
    if(button == GLFW_MOUSE_BUTTON_RIGHT){
      engine->removeObject3d(cursor);
      cursor = NULL;
    }else if(button == GLFW_MOUSE_BUTTON_LEFT){ // TODO: map strings to constructors
      if(cursor->enoughResources()){
        vec3 pos = obj->getPosition();
        string name = ((Building*)cursor)->getName();
        Building* building = BuildingFactory::newByName(name, vec3(pos.x, pos.y, 0));
        engine->addObject3d(building);
        engine->makeClickable(building, true);

        myResources.update(myResources - building->getCost());
      }
    }

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
  
  cursor = new Cursor(name, vec3(0, 0, 0), &myResources);
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


