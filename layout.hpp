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
	GridLayout(vec2 pos, vec2 size, float width, map<string, string> properties) : Quad(pos) {
		this->elementSize = size;
		this->width = width;
		if(contains(properties, string("padding")) )
			this->padding = stoi(properties["padding"]);
		if(contains(properties, string("margin")) )
			this->margin = stoi(properties["margin"]);
		if(contains(properties, string("align")) ){
			if(properties["align"] == "left")
				this->align = LEFT;
			else if(properties["align"] == "center")
				this->align = CENTER;
			else if(properties["align"] == "right")
				this->align = RIGHT;
		}
	}
	~GridLayout();

	void draw(float elapsed, ShaderData& shader, mat4* MVP){
		//margin * 2 + (padding+size.x) * (n-1) + size.x <= width
	}
	void setElements(vector<Drawable2d*> elements){
		this->elements = elements;
		
		int n = (width - margin * 2 - elementSize.x) / (padding+elementSize.x) + 1;
		int offset;
		if(align == RIGHT)
			offset =  width - (margin * 2 + padding * (n-1) + elementSize.x * n);
		else if(align == LEFT)
			offset = 0;
		else
			offset = (width - (margin * 2 + padding * (n-1) + elementSize.x * n)) / 2;
		
		for(int i=0; i<(signed)elements.size(); i++){
			elements[i]->setPosition(vec3(pos.x + offset + margin + (i%n) * (padding+elementSize.x), pos.y - (i/n)*(padding+elementSize.y), 0));
			//elements[i]->draw(elapsed, shader, MVP);
		}
		size = vec2(width, (elements.size()/n-1) * (padding + size.y) + size.y);
	}
private:
vec2 elementSize;
float width, padding = 0, margin = 0;
Align align = CENTER;
vector<Drawable2d*> elements;
};