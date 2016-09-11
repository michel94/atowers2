#include "maploader.hpp"
#include "terrain.hpp"

map<pair<int, int>, double> MapLoader::noises;

Cube*** MapLoader::getRandomMap(int amplitude, int width, int height){
  double** heights = MapLoader::generateMap(amplitude, width, height);
  Cube*** cubeMap = new Cube**[height];
  for(int i=0; i<height; i++){
    cubeMap[i] = new Cube*[width];
    for(int j=0; j<width; j++){
      cubeMap[i][j] = new Grass(vec3(i, j, heights[i][j]));
    }
  }
  return cubeMap;
}

double** MapLoader::generateMap(int amplitude, int width, int height){
	MapLoader::noises.clear();
	double** noise = new double*[height];
	for (int i = 0; i < height; i++)
    	noise[i] = new double[width];

	for(int i = 0; i < height; i++){
		for(int j = 0; j < width; j++){
			noise[i][j] = generateHeight(i, j, amplitude);
		}
	}
	return noise;
}

double MapLoader::generateHeight(int x, int y, int amplitude){
	return getSmoothNoise(x, y) * amplitude;
}

double MapLoader::getSmoothNoise(int x, int y){
	double corners = (getNoise(x - 1, y - 1) + getNoise(x + 1, y - 1) + getNoise(x - 1, y + 1) + getNoise(x + 1, y + 1)) / 16;
	double sides = (getNoise(x - 1, y) + getNoise(x + 1, y)  + getNoise(x, y - 1) + getNoise(x, y + 1)) / 8;
	double center = getNoise(x, y) / 4;
	return (corners + sides + center);
}

double MapLoader::getNoise(int x, int y){
	if(MapLoader::noises.find(make_pair(x,y)) != MapLoader::noises.end())
		return MapLoader::noises[make_pair(x,y)];
	else{
		MapLoader::noises[make_pair(x,y)] = static_cast <double> (rand()) / (static_cast <double> (RAND_MAX/2)) - 1;
		return MapLoader::noises[make_pair(x,y)];
	}
}
