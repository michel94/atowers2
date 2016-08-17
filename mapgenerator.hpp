#include <time.h>
#include <stdlib.h>
#include <map>
#include <utility>

using namespace std;

class MapGenerator{
public:
	static double** generateMap(int amplitude, int width, int height); 
private:
	static map<pair<int, int>, double> noises;
	static double generateHeight(int x, int y, int amplitude);
	static double getSmoothNoise(int x, int y);
	static double getNoise(int x, int y);
};
