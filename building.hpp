#include "resources.hpp"
#include "Engine/object.hpp"

class Building : public Object {
public:
  explicit Building(string name, vec3 pos) : Object("buildings/" + name, pos){
    this->name = name;
    setShader("blended");
  };
  virtual ~Building(){;}
  void onClick(GameLogic* game, int button, int action, int mods){
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
  virtual Resources getCost(){
  	return cost;
  }

  string getName(){
    return name;
  }

  bool overred = false;
private:
  vec4 colorMix = vec4(1.5, 0.5, 0.5, 1.0);

protected:
  string name;
  Resources cost;
  vec2 dimensions = vec2(1,1);

};

class Hut : public Building {
public:
	Hut(vec3 pos) : Building("hut", pos){
		cost = Resources(1, 4, 1, 5, 3);
	}
};

class Rock : public Building {
public:
	Rock(vec3 pos) : Building("rock", pos){
		cost = Resources(0, 0, 0, 0, 0);
	}
};

class Tower : public Building {
public:
	Tower(vec3 pos) : Building("tower", pos){
		cost = Resources(0, 2, 1, 10, 14);
		dimensions = vec2(2, 2);
	}
};

class Palisade : public Building {
public:
	Palisade(vec3 pos) : Building("palisade", pos){
		cost = Resources(0, 1, 0, 10, 0);
		dimensions = vec2(1, 1);
	}
};

class Palmtree : public Building {
public:
	Palmtree(vec3 pos) : Building("palmtree", pos){
		cost = Resources(0, 0, 0, 1, 0);
		dimensions = vec2(1, 1);
	}
};

const vector<string> buildings = {"castle", "farm", "hut", "tower", "rock", "palisade", "palmtree"};
class BuildingFactory{
public:
	static Building* newByName(string name, vec3 pos){
		if(name == "hut")
			return (Building*) new Hut(pos);
		else if(name == "rock")
			return (Building*) new Rock(pos);
		else if(name == "tower")
			return (Building*) new Tower(pos);
		else if(name == "palisade")
			return (Building*) new Palisade(pos);
		else if(name == "palmtree")
			return (Building*) new Palmtree(pos);
		else
			return (Building*) new Rock(pos);
	}
	
	static Resources getCost(string name){
		Building* b = newByName(name, vec3(0,0,0));
		Resources r = b->getCost();
		delete b;
		return r;
	}
};
