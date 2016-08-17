#include "mapgenerator.hpp"

map<pair<int, int>, double> MapGenerator::noises;

double** MapGenerator::generateMap(int amplitude, int width, int height){
	MapGenerator::noises.clear();
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

double MapGenerator::generateHeight(int x, int y, int amplitude){
	return getSmoothNoise(x, y) * amplitude;
}

double MapGenerator::getSmoothNoise(int x, int y){
	double corners = (getNoise(x - 1, y - 1) + getNoise(x + 1, y - 1) + getNoise(x - 1, y + 1) + getNoise(x + 1, y + 1)) / 16;
	double sides = (getNoise(x - 1, y) + getNoise(x + 1, y)  + getNoise(x, y - 1) + getNoise(x, y + 1)) / 8;
	double center = getNoise(x, y) / 4;
	return (corners + sides + center);
}

double MapGenerator::getNoise(int x, int y){
	if(MapGenerator::noises.find(make_pair(x,y)) != MapGenerator::noises.end())
		return MapGenerator::noises[make_pair(x,y)];
	else{
		MapGenerator::noises[make_pair(x,y)] = static_cast <double> (rand()) / (static_cast <double> (RAND_MAX/2)) - 1;
		return MapGenerator::noises[make_pair(x,y)];
	}
}