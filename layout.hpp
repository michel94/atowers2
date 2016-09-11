#include "Engine/quad.hpp"

template<class K, class V>
inline bool contains(map<K, V> &m, K k){
	return m.find(k) != m.end();
}

enum Align{
	CENTER, RIGHT, LEFT
};

class GridLayout : Quad {

public:
  GridLayout(vec2 pos, vec2 size, float width, map<string, string> properties);
	~GridLayout();
  void setElements(vector<Drawable2d*> elements);

	void draw(float elapsed, ShaderData& shader, mat4* MVP){
		//margin * 2 + (padding+size.x) * (n-1) + size.x <= width
	}
private:
vec2 elementSize;
float width, padding = 0, margin = 0;
Align align = CENTER;
vector<Drawable2d*> elements;
};
