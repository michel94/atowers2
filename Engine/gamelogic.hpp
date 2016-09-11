#pragma once

#include "drawable.hpp"

class Drawable;
class Drawable2d;

class GameLogic{
public:
	// 3d events
	virtual void onOver(Drawable* obj) = 0; 
	virtual void onClick(Drawable* obj, int button, int action, int mods) = 0;
	// 2d events
	virtual void onMenuOver(Drawable2d* obj) = 0;
	virtual void onMenuClick(Drawable2d* obj, int button, int action, int mods) = 0;
};
