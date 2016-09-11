#include <time.h>
#include <stdlib.h>
#include <map>
#include <utility>
#include "Engine/cube.hpp"

using namespace std;

class MapLoader{
public:
  static Cube*** getRandomMap(int amplitude, int width, int height);
	static double** generateMap(int amplitude, int width, int height); 
private:
	static map<pair<int, int>, double> noises;
	static double generateHeight(int x, int y, int amplitude);
	static double getSmoothNoise(int x, int y);
	static double getNoise(int x, int y);
};
