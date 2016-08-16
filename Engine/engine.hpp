#pragma once

#include "opengl.h"
#include <GLFW/glfw3.h>
#include "shaders.hpp"
#include "terrain.hpp"
#include "object.hpp"
#include <vector>
#include <set>

using namespace std;


class Engine{
public:
  Engine(GameLogic* game, int width, int height);
  ~Engine();
  void loadMap(double** matrix, int, int);
  void run();

  int getColorId();

  void addObject3D(Drawable* obj);
  void addObject2D(Drawable2d* obj);
  void makeClickable(Drawable*, bool);
  void makeClickable(Drawable2d*, bool);
  
  int SCREEN_WIDTH, SCREEN_HEIGHT;
  bool click2d;
  GameLogic* game = NULL;

  GameLogic* getGame();
  GLFWwindow* getWindow();
private:
  float initialWidth, initialHeight;
  GLFWwindow* window;

  Cube*** terrain;
  int mapWidth, mapHeight;
  int colorId = 0;

  int frame = 0;
  double timebase = 0, tm = 0, last_tick;
  double fps;

  // Camera
  float angleX = 45, angleY = -45, posX=0, posY=-1, posZ = 4, zoom = 0.8;
  int mouseX, mouseY;
  GLuint frameBuffer = 0;

  vector<Drawable*> clickable3dObjects;
  vector<Drawable2d*> clickable2dObjects;

  vector<Drawable*> drawable3dObjects;
  vector<Drawable2d*> drawable2dObjects;

  map<string, GLuint> uniforms;
  GLuint program;

  static void mouseCallback(GLFWwindow* window, int button, int action, int mods);
  static void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
  static void cursorMoveCallback(GLFWwindow* window, double xpos, double ypos);
  static void scrollCallback(GLFWwindow* window, double dx, double dy);
  static void resizeCallback(GLFWwindow* window, int width, int height);

  bool pendingClick = false;
  Drawable* getCurrentClickable();
  void handleClick3d(mat4 MVP, int windowWidth, int windowHeight);
  void handleClick2d(int windowWidth, int windowHeight);
  void drawTriangles(Drawable* obj, unsigned int color);

  void openglInit();
  void render2d(float elapsed, int windowWidth, int windowHeight);
  void render3d(float elapsed, int windowWidth, int windowHeight);
  void updateCamera(float);

  float elapsedTime();
  void showFPS(float elapsed);

  float totalTime = 0.0f;
  int frameCount = 0;
  int boardTexture;

};

