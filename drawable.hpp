#include "opengl.h"
#include "utils.hpp"
#include <vector>
#include <glm/glm.hpp>

using namespace std;
using namespace glm;

class Drawable{
public:
	virtual void draw(mat4* MVP){};
	void drawTexturedQuads(GLuint elementsVBO, GLuint verticesVBO, int nVertices, GLuint texId, GLuint texCoordsVBO);
	void drawTexturedTriangles(GLuint elementsVBO, GLuint verticesVBO, int nVertices, GLuint texId, GLuint texCoordsVBO);
	vec3& getPosition();
	GLuint MVPid;
	mat4 MVP;
protected:
	vec3 pos = vec3(0,0,0);
};