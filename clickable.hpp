#include "drawable.hpp"

class Triangle{
public:
	Triangle(vec3 p1, vec3 p2, vec3 p3){
		this->p1 = p1;
		this->p2 = p2;
		this->p3 = p3;
	}
private:
	vec3 p1, p2, p3;
};

class Clickable : Drawable{
public:
	virtual vector<Triangle> getTriangles() = 0;
	void drawTexturedQuads(vector<float> vertices, GLint texId, vector<float> texCoords){
		return Drawable::drawTexturedQuads(vertices, texId, texCoords);
	}

};

