
#include "opengl.h"
#include <GLFW/glfw3.h>
#include "shaders.hpp"
#include "terrain.hpp"
#include "clickable.hpp"
using namespace std;


class Engine{
public:
  Engine(int width, int height);
  ~Engine();
  void loadMap(double** matrix, int, int);
  void run();

  void render();
  
  int getColorId();
private:
  GLFWwindow* window;
  int SCREEN_WIDTH, SCREEN_HEIGHT;

  Cube*** terrain;
  int mapWidth, mapHeight;
  int colorId = 0;

  int frame = 0, windowWidth, windowHeight;
  double timebase = 0, tm = 0, last_tick;
  double fps;

  // Camera
  float angleX = 45, angleY = -45, posX=0, posY=-1, posZ = 4, zoom = 1;
  int mouseX, mouseY;
  GLuint frameBuffer = 0;

  vector<Clickable*> clickableObjects;
  vector<Drawable*> drawableObjects;

  Clickable* getCurrentClickable();

  void addObject(Drawable* obj){
    printf("Added Drawable\n"); //TODO
  }
  void addObject(Clickable* obj){
    addObject((Drawable*) obj);
    printf("Added Clickable\n"); //TODO
  }

  void updateCamera();
  void openglInit();
  static void mouseCallback(GLFWwindow* window, int button, int action, int mods);
  static void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
  static void cursorMoveCallback(GLFWwindow* window, double xpos, double ypos);
};

