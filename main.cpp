
#include <iostream>
#include "main.hpp"
#include "mapgenerator.hpp"
#include <glm/glm.hpp>
#include <string.h>
#include "engine.hpp"
#include "quad.hpp"

#define SCREEN_SIZE_CUT 1.2f

using namespace std;
using namespace glm;

int SCREEN_WIDTH = 1200*2, SCREEN_HEIGHT = 675*2;
Engine *engine;

void monitorResolution(int *w, int *h){
  int count;
  const GLFWvidmode* modes = glfwGetVideoModes(glfwGetPrimaryMonitor(), &count);

  int maxWidth = 0; int maxHeight = 0;
  for(int i = 0; i < count; i++){
    if(modes[i].width > maxWidth)
      maxWidth = modes[i].width;
    if(modes[i].height > maxHeight)
      maxHeight = modes[i].height;
  }

  *w = maxWidth  / SCREEN_SIZE_CUT;
  *h = maxHeight / SCREEN_SIZE_CUT;
}

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

  Quad *farmButton = new Quad(Loader::loadPng("menu/castle.png"), vec2(1000.0, 600.0), vec2(128, 128));
  engine->addObject2D(farmButton);
}

int main(int argc, char **argv){
  glfwInit();
  //monitorResolution(&SCREEN_WIDTH, &SCREEN_HEIGHT);
  engine = new Engine(SCREEN_WIDTH, SCREEN_HEIGHT);
  
  loadScene();

  engine->run();

  glfwTerminate();

	return 0;
}
