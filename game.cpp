
#include <iostream>
#include "game.hpp"
#include "maploader.hpp"
#include "button.hpp"
#include "Engine/textbox.hpp"
#include "layout.hpp"

#define SCREEN_SIZE_CUT 1.2f

using namespace std;
using namespace glm;

const int SCREEN_WIDTH = 1200, SCREEN_HEIGHT = 675;

void Game::loadScene(){
  int mapHeight = 50, mapWidth = 50;
  engine->loadMap(MapLoader::getRandomMap(2, mapHeight, mapWidth), mapHeight, mapWidth);
  
  for(int i=0; i<10; i++){
    Building* building = new Building("hut", vec3(rand()%mapWidth, rand()%mapHeight, 0.0f));
    engine->addObject3d(building);
  }

  myResources = Resources(10, 27, 20, 1000, 10);

  vec2 size = vec2(64, 64);
  GridLayout* grid = new GridLayout(
    vec2(0.8*SCREEN_WIDTH, SCREEN_HEIGHT -200),
    size,
    0.2f*SCREEN_WIDTH,
    {{"padding", "10"}, {"margin", "10"}, {"align", "center"}}
  );

  vector<Drawable2d*> buttons(buildings.size());
  for(int i=0; i<(signed)buildings.size(); i++){
    Button* button = new Button(buildings[i], vec2(0,0), vec2(64, 64));
    engine->addObject2d(button);
    buttons[i] = button;
  }
  grid->setElements(buttons);

  
  vector<string> resourceNames = Resources::getResourceList();
  vec2 sizeIcon = vec2(32, 32);
  GridLayout* gridRes = new GridLayout(
    vec2(0.8*SCREEN_WIDTH, SCREEN_HEIGHT -40),
    sizeIcon,
    0.2f*SCREEN_WIDTH,
    {{"padding", "12"}, {"margin", "8"}, {"align", "center"}}
  );

  vector<Textbox*> textboxes(resourceNames.size());
  vector<Drawable2d*> groups(resourceNames.size());
  for(int i=0; i < (signed) resourceNames.size(); i++){
    Image* image = new Image(resourceNames[i], vec2(0, 0), sizeIcon);
    Textbox* textbox = new Textbox(vec2(7, -13), 120, "");

    Group2d* g = new Group2d(vec3(0, 0, 0), {image, textbox});
    groups[i] = g;
    engine->addObject2d(g);
    
    textboxes[i] = textbox;

  }
  myResources.setTextboxes(textboxes);
  gridRes->setElements(groups);

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
    if(cursor->pressing){
      vec3 v = obj->getPosition() - cursor->getPosition();
      cursor->setAngle(atan(v.y, v.x) + radians(90.0f));
    }else{
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
}

void Game::onClick(Drawable* obj, int button, int action, int mods){
  if(cursor){
    if(button == GLFW_MOUSE_BUTTON_RIGHT){
      if(cursor->over)
        cursor->over->overred = false;
      engine->removeObject3d(cursor);
      cursor = NULL;
    }else if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
      cursor->pressing = true;
    }else if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE){ // TODO: map strings to constructors
      Resources cost = BuildingFactory::getCost(cursor->getName());
      if(myResources.enoughResources(cost)){
        vec3 pos = cursor->getPosition();
        string name = ((Building*)cursor)->getName();
        Building* building = BuildingFactory::newByName(name, vec3(pos.x, pos.y, 0));
        building->setAngle(cursor->getAngle());
        if(!engine->addObject3d(building)){
          delete building;
          cursor->pressing = false;
          return;
        }

        myResources.update(myResources - building->getCost());
      }
      cursor->pressing = false;
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

void Game::onMenuClick(Drawable2d* obj, int button, int action, int mods){
}

int main(int argc, char **argv){
  new Game();

  //GridLayout grid = new GridLayout(pos, size, width, {{"padding", "5"}, {"align", "center"}, {"margin", "5"}});
  //grid.addElements(elements);
  //map<string, int> properties = {{"padding", 5}}
  
	return 0;
}


