#include "opengl.h"
#include <vector>
#include <glm/glm.hpp>

using namespace std;
using namespace glm;


class Drawable{
public:
	virtual void draw() = 0;
	//void renderTriangles(vector<> vertices, GLint texId, vector<> texCoords);
	void drawTexturedQuads(vector<float> vertices, GLint texId, vector<float> texCoords);
	//void renderColoredQuads(vector<> vertices, int color);
protected:
	vec3 pos = vec3(0,0,0);
};