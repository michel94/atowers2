#include "opengl.h"
#include <vector>
#include <glm/glm.hpp>

using namespace std;
using namespace glm;


class Drawable{
public:
	virtual void draw(){};
	void drawTexturedQuads(GLuint verticesVBO, int nVertices, GLuint texId, GLuint texCoordsVBO);
	GLuint createVBO(vector<float> data);
protected:
	vec3 pos = vec3(0,0,0);
};