#include "terrain.hpp"
#include "loader.hpp"

Grass::Grass(vec3 pos) : Cube(Loader::loadPng("grass/grass.png"), pos){
  setClickable(true);
}

void Grass::onClick(GameLogic* game, int button, int action, int mods){
  //printf("Click grass on position %f, %f\n", pos.x, pos.y);
}