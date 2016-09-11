#pragma once

#include "Engine/cube.hpp"

class Grass : public Cube {
public:
	Grass(vec3 pos);
	void onClick(GameLogic* game, int button, int action, int mods);
};
