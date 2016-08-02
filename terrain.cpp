#include "terrain.hpp"

#include "loader.hpp"

Grass::Grass(vec3 pos) : Cube(Loader::loadPng("grass/grass.png"), pos){
  ;
}

void Grass::onClick(){
  printf("Click grass on position %f, %f\n", pos.x, pos.y);
}