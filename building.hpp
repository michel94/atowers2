#include "resources.hpp"

class Building : public Object {
public:
  explicit Building(string name, vec3 pos) : Object("buildings/" + name, pos){
    this->name = name;
    setShader("blended");
  };
  ~Building(){;}
  void onClick(GameLogic* game, int button){
    printf("Clicked on %s\n", name.c_str());
  }
  virtual void draw(float elapsed, ShaderData& shader, mat4* MVP){
    if(overred){
      glUniform4fv(shader["colorMix"], 1, &colorMix[0]);
    }else{
      vec4 v = vec4(1.0, 1.0, 1.0, 1.0);
      glUniform4fv(shader["colorMix"], 1, &v[0]);
    }
    Object::draw(elapsed, shader, MVP);
  }

  string getName(){
    return name;
  }

  bool overred = false;
private:
  vec4 colorMix = vec4(1.5, 0.5, 0.5, 1.0);

protected:
  string name;
  Resources resources;

};

class Hut : public Building {
public:
	Hut(vec3 pos) : Building("hut", pos){

	}
};

class Rock : public Building {
public:
	Rock(vec3 pos) : Building("rock", pos){

	}
};

class Tower : public Building {
public:
	Tower(vec3 pos) : Building("tower", pos){

	}
};

class BuildingFactory{
public:
	static Building* newBuildingByName(string name, vec3 pos){
		if(name == "hut")
			return (Building*) new Hut(pos);
		else if(name == "rock")
			return (Building*) new Rock(pos);
		else if(name == "tower")
			return (Building*) new Tower(pos);
	}
	
};
