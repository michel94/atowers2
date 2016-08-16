#include "Engine/quad.hpp"

class Button : public Quad{
public:
  Button(string name, vec2 pos, vec2 size) : Quad(Loader::loadPng("buttons/" + name + ".png"), pos, size) {
    this->name = name;
  }
  void onClick(GameLogic* game){
    cout << "Clicked on button " << name << endl;
  }
private:
  string name;
};
