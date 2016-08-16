
#include <iostream>
#include "main.hpp"
#include "mapgenerator.hpp"
#include <glm/glm.hpp>
#include <string.h>
#include "Engine/engine.hpp"
#include "Engine/quad.hpp"

#define SCREEN_SIZE_CUT 1.2f

using namespace std;
using namespace glm;

int SCREEN_WIDTH = 1200*2, SCREEN_HEIGHT = 675*2;
Engine *engine;

class Castle : public Object {
public: 
  Castle(vec3 pos) : Object("castle", pos){
  };
  ~Castle();
  void onClick(){
    printf("Clicked on castle!\n");
  }
};

void loadScene(){
  int mapHeight = 30, mapWidth = 30;
  MapGenerator a;
  engine->loadMap(a.generateMap(2, mapHeight, mapWidth), mapHeight, mapWidth);
  
  for(int i=0; i<10; i++){
    Object* castle = new Castle(vec3(rand()%mapWidth, rand()%mapHeight, 0.0f));
    engine->addObject3D(castle);
    engine->makeClickable(castle, true);
  }

  vec2 size = vec2(128, 128);
  Button* farmButton = new Button("castle", vec2(SCREEN_WIDTH, SCREEN_HEIGHT) - size, size);
  engine->addObject2D(farmButton);
  engine->makeClickable(farmButton, true);
}

int main(int argc, char **argv){
  engine = new Engine(SCREEN_WIDTH, SCREEN_HEIGHT);
  loadScene();

  engine->run();
  
	return 0;
}
