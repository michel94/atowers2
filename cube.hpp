#include "drawable.hpp"

class Cube : Drawable{
public:
	Cube(int, int);
	virtual void draw();

private:
	int topTexture, sideTexture;
	void drawSquare(bool flip, int texture);
};