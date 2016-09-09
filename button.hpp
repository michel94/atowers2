#include "Engine/quad.hpp"
#include "game.hpp"

class Game;

class Button : public Quad{
public:
  Button(string name, vec2 pos, vec2 size) : Quad(Loader::loadPng("buttons/" + name + ".png"), pos, size) {
    this->name = name;
  }
  void onClick(GameLogic* gameLogic, int button){
  	Game* game = (Game*) gameLogic;
  	game->onButtonClick(name);
    cout << "Clicked on button " << name << endl;
  }
private:
  string name;
};

class Image : public Quad{
public:
  Image(string name, vec2 pos, vec2 size) : Quad(Loader::loadPng("menu/" + name + ".png"), pos, size) {
    this->name = name;
  }
private:
  string name;
};
