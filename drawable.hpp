#pragma once

#include "opengl.h"
#include "utils.hpp"
#include <vector>
#include <glm/glm.hpp>

using namespace std;
using namespace glm;

struct EngineData{
	bool is2d;
};

class Drawable{
public:
	virtual void draw(mat4* MVP){};
	void drawTexturedQuads(GLuint elementsVBO, GLuint verticesVBO, int nVertices, GLuint texId, GLuint texCoordsVBO);
	void drawTexturedTriangles(GLuint elementsVBO, GLuint verticesVBO, int nVertices, GLuint texId, GLuint texCoordsVBO);
	void drawTexturedPolygons(GLuint type, GLuint elementsVBO, GLuint verticesVBO, int nVertices, GLuint texId, GLuint texCoordsVBO);
	vec3& getPosition();
	bool isClickable();
	void setClickable(bool);
	
	virtual vector<Triangle> getTriangles(){return vector<Triangle>();}
	virtual void onClick(){;}

	GLuint MVPid;
	mat4 MVP;

	EngineData engineData;
protected:
	vec3 pos = vec3(0,0,0);
	bool clickable = false;
};
