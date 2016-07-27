#include "cube.hpp"

void Cube::draw(){
  glTranslatef(pos.x, pos.y, pos.z);
  Clickable::drawTexturedQuads(vertices, sideTexture, texCoords);
  Clickable::drawTexturedQuads(topVertices, topTexture, texCoords);
}

vector<Triangle> Cube::getTriangles(){
  return triangles;
}

void Cube::addTriangle(vector<float> &v, int i) {
  auto v1 = vec3(v[ i ], v[i+1 ], v[i+2 ]),
       v2 = vec3(v[i+3], v[i+4 ], v[i+5 ]),
       v3 = vec3(v[i+6], v[i+7 ], v[i+8 ]),
       v4 = vec3(v[i+9], v[i+10], v[i+11]);
  this->triangles.push_back(Triangle(v1, v2, v3));
  this->triangles.push_back(Triangle(v1, v3, v4));
}

Cube::Cube(int topTexture, int sideTexture, vec3 pos){
  this->topTexture = topTexture;
  this->sideTexture = sideTexture;
  this->pos = pos;

  auto &v = vertices;
  for(int i=0; i<(signed)v.size(); i+=12){
    addTriangle(vertices, i);
  }
  auto &v2 = topVertices;
  for(int i=0; i<(signed)v2.size(); i+=12){
    addTriangle(v2, i);
  }


}
