#include "cube.hpp"
#include <glm/gtc/matrix_transform.hpp>

void Cube::draw(mat4* MVP){
  mat4 tr = glm::translate(glm::mat4(), glm::vec3(pos.x, pos.y, pos.z));
  mat4 myMVP = *MVP * tr;
  glUniformMatrix4fv(MVPid, 1, GL_FALSE, &myMVP[0][0]);
  Drawable::drawTexturedQuads(verticesVBO, vertices.size(), sideTexture, texCoordsVBO);
  Drawable::drawTexturedQuads(topVerticesVBO, topVertices.size(), topTexture, texCoordsVBO);
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

  verticesVBO = Drawable::createVBO(vertices);
  texCoordsVBO = Drawable::createVBO(texCoords);
  topVerticesVBO = Drawable::createVBO(topVertices);
}
