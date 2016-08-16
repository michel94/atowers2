#pragma once

#include "cube.hpp"

class Grass : public Cube {
public:
	Grass(vec3 pos);
	void onClick(GameLogic* game);
};
