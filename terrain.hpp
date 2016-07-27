#include "cube.hpp"

class Grass : public Cube {
	public:
	Grass(vec3 pos) : Cube(Loader::loadPng("grass/top.png"), Loader::loadPng("grass/side.png"), pos){;}
	
};
