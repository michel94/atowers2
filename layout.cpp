
#include "layout.hpp"

GridLayout::GridLayout(vec2 pos, vec2 size, float width, map<string, string> properties) : Quad(pos) {
  this->elementSize = size;
  this->width = width;
  if(hasKey(properties, string("padding")) )
    this->padding = stoi(properties["padding"]);
  if(hasKey(properties, string("margin")) )
    this->margin = stoi(properties["margin"]);
  if(hasKey(properties, string("align")) ){
    if(properties["align"] == "left")
      this->align = LEFT;
    else if(properties["align"] == "center")
      this->align = CENTER;
    else if(properties["align"] == "right")
      this->align = RIGHT;
  }
}
void GridLayout::setElements(vector<Drawable2d*> elements){
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
