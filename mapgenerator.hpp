#include "opengl.h"
#include <time.h>
#include <stdlib.h>
#include <map>
#include <utility>

using namespace std;

class MapGenerator{
public:
	double** generateMap(int amplitude, int width, int height); 
private:
	map<pair<int, int>, double> noises;
	double generateHeight(int x, int y, int amplitude);
	double getSmoothNoise(int x, int y);
	double getNoise(int x, int y);
};