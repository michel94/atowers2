#include "Engine/textbox.hpp"

class Resources{
public:
  Resources(){
    this->bone = 0;
    this->skin = 0;
    this->food = 0;
    this->wood = 0;
    this->stone = 0;
  }
  Resources(int bone, int skin, int food, int wood, int stone){
    this->bone = bone;
    this->skin = skin;
    this->food = food;
    this->wood = wood;
    this->stone = stone;
  }
  static vector<string> getResourceList(){
    return {"bone", "skin", "food", "wood", "stone"};
  }
  int getBone(){
    return bone;
  }
  void setBone(int bone){
    this->bone = bone;
    if(boneT)
      boneT->setText(to_string(bone));
  }
  int getSkin(){
    return skin;
  }
  void setSkin(int skin){
    this->skin = skin;
    if(skinT)
      skinT->setText(to_string(skin));
  }
  int getFood(){
    return food;
  }
  void setFood(int food){
    this->food = food;
    if(foodT)
      foodT->setText(to_string(food));
  }
  int getWood(){
    return wood;
  }
  void setWood(int wood){
    this->wood = wood;
    if(woodT)
      woodT->setText(to_string(wood));
  }
  int getStone(){
    return stone;
  }
  void setStone(int stone){
    this->stone = stone;
    if(stoneT)
      stoneT->setText(to_string(stone));
  }

  void setTextboxes(vector<Textbox*> t){
    try{
      this->boneT = t[0];
      this->skinT = t[1];
      this->foodT = t[2];
      this->woodT = t[3];
      this->stoneT = t[4];
      printf("OK\n");
    }catch(...){
      ;
    }
    update(*this);
  }

  void update(Resources r){
    setBone(r.getBone());
    setSkin(r.getSkin());
    setFood(r.getFood());
    setWood(r.getWood());
    setStone(r.getStone());
  }

  bool isPositive(){
    return bone >= 0 && skin >= 0 && food >= 0 && wood >= 0 && stone >= 0;
  }

private:
  int bone, skin, food, wood, stone;
  Textbox *skinT=NULL, *boneT=NULL, *foodT=NULL, *woodT=NULL, *stoneT=NULL;

};
Resources operator+ (Resources& r1, Resources r2) {
  return Resources(r1.getBone() + r2.getBone(), r1.getSkin() + r2.getSkin(), r1.getFood() + r2.getFood(), r1.getWood() + r2.getWood(), r1.getStone() + r2.getStone());
}
Resources operator- (Resources& r1, Resources r2) {
  return Resources(r1.getBone() - r2.getBone(), r1.getSkin() - r2.getSkin(), r1.getFood() - r2.getFood(), r1.getWood() - r2.getWood(), r1.getStone() - r2.getStone());
}
