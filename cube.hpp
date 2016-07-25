#include "clickable.hpp"

class Cube : public Clickable{
public:
	Cube(int, int);
	virtual void draw();
	vector<Triangle> getTriangles();

private:
	int topTexture, sideTexture;
	vector<Triangle> triangles;
	vector<float> vertices = 
         { 0.0f, 1.0f, 0.0f,
           0.0f, 1.0f, 1.0f,
           0.0f, 0.0f, 1.0f,
           0.0f, 0.0f, 0.0f,
         1.0f, 1.0f, 0.0f,
         1.0f, 1.0f, 1.0f,
         0.0f, 1.0f, 1.0f,
         0.0f, 1.0f, 0.0f,
           1.0f, 0.0f, 0.0f,
           1.0f, 0.0f, 1.0f,
           1.0f, 1.0f, 1.0f,
           1.0f, 1.0f, 0.0f,
          0.0f, 0.0f, 0.0f,
          0.0f, 0.0f, 1.0f,
          1.0f, 0.0f, 1.0f,
          1.0f, 0.0f, 0.0f
         };
  vector<float> texCoords = 
          {
            0.0f,0.0f, 0.0f,1.0f, 1.0f,1.0f, 1.0f,0.0f,
            0.0f,0.0f, 0.0f,1.0f, 1.0f,1.0f, 1.0f,0.0f,
            0.0f,0.0f, 0.0f,1.0f, 1.0f,1.0f, 1.0f,0.0f,
            0.0f,0.0f, 0.0f,1.0f, 1.0f,1.0f, 1.0f,0.0f 
          };
  vector<float> topVertices = {
    0.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 0.0f, 1.0f,
  };
  vector<float> topTexCoords = {0.0f,0.0f, 0.0f,1.0f, 1.0f,1.0f, 1.0f,0.0f};

  void addTriangle(vector<float> &v, int i);
};

