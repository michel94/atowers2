#pragma once

#include "opengl.h"
#include "gamelogic.hpp"
#include "shaders.hpp"
#include <vector>
#include <glm/glm.hpp>

using namespace std;
using namespace glm;

struct Properties{
	bool is2d;
};

class Triangle{
public:
	Triangle(vec3 p1, vec3 p2, vec3 p3){
		this->p1 = p1;
		this->p2 = p2;
		this->p3 = p3;
	}
	vec3 p1, p2, p3;
};

class Drawable{
public:
	virtual void draw(float elapsed, ShaderData& shader, mat4* MVP) = 0;
	void drawTexturedQuads(GLuint elementsVBO, GLuint verticesVBO, int nVertices, GLuint texId, GLuint texCoordsVBO);
	void drawTexturedTriangles(GLuint elementsVBO, GLuint verticesVBO, int nVertices, GLuint texId, GLuint texCoordsVBO);
	void drawTexturedPolygons(GLuint type, GLuint elementsVBO, GLuint verticesVBO, int nVertices, GLuint texId, GLuint texCoordsVBO);
	vec3& getPosition();
	bool isClickable();
	void setClickable(bool);
	string getShader();
	void setShader(string shader);

	virtual vector<Triangle> getTriangles(){return vector<Triangle>();}
	virtual void onClick(GameLogic*, int) = 0;

	virtual Properties* getProperties();
protected:
	vec3 pos = vec3(0,0,0);
	bool clickable = false;

	Properties properties = {false};
	string SHADER = "textured";
};

class Drawable2d : public Drawable{
public:
	virtual vec2 getSize() = 0;

	Properties* getProperties(){
		return &properties;
	}
protected:
	Properties properties = {true};
};
