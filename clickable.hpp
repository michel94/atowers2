#pragma once

#include "drawable.hpp"
#include <stdio.h>

class Triangle{
public:
	Triangle(vec3 p1, vec3 p2, vec3 p3){
		this->p1 = p1;
		this->p2 = p2;
		this->p3 = p3;
	}
	vec3 p1, p2, p3;
private:
};

class Clickable : public Drawable{
public:
	virtual vector<Triangle> getTriangles() = 0;
	void drawTriangles(unsigned int color){
    vector<Triangle> triangles = getTriangles();
		GLubyte r = color & 0x000000FF, g = (color & 0x0000FF00) >> 8,
		        b = (color & 0x00FF0000) >> 16, a = (color & 0xFF000000) >> 24;
    
      //printf("WTF %d %d %d\n", r, g, b);
    if(g == 0 && color > 300)
      printf("WTF\n");
    glColor4ub(r, g, b, a);
    glTranslatef(pos.x, pos.y, pos.z);
    glBegin(GL_TRIANGLES);
    for(int i=0; i<(signed)triangles.size(); i++){
      glVertex3f(triangles[i].p1.x, triangles[i].p1.y, triangles[i].p1.z);
      glVertex3f(triangles[i].p2.x, triangles[i].p2.y, triangles[i].p2.z);
      glVertex3f(triangles[i].p3.x, triangles[i].p3.y, triangles[i].p3.z);
    }
    glEnd();
	}
  virtual void onClick(){;}

};

