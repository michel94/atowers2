#include "Engine/textbox.hpp"

class Resources{
public:
  Resources(){
    this->bone = 0;
    this->skin = 0;
  }
  Resources(int bone, int skin){
    this->bone = bone;
    this->skin = skin;
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
  void setTextboxes(vector<Textbox*> t){
    this->boneT = t[0];
    this->skinT = t[1];
    update(*this);
  }

  void update(Resources r){
    setBone(r.getBone());
    setSkin(r.getSkin());
  }

private:
  int bone, skin;
  Textbox *skinT=NULL, *boneT=NULL;

};
Resources operator+ (Resources& r1, Resources& r2) {
  return Resources(r1.getBone() + r2.getBone(), r1.getSkin() + r2.getSkin());
}
Resources operator- (Resources& r1, Resources& r2) {
  return Resources(r1.getBone() - r2.getBone(), r1.getSkin() - r2.getSkin());
}
