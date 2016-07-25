#include <GL/glew.h>
#include <GL/glu.h>
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
};