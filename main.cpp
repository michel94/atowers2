
#include <iostream>
#include "main.hpp"
#include "loader.hpp"
#include "shaders.hpp"
#include "cube.hpp"
#include <glm/glm.hpp>

#define SCREEN_SIZE_CUT 1.2f

using namespace std;
using namespace glm;

int frame = 0, windowWidth, windowHeight;
double timebase = 0, tm = 0, last_tick;
double fps;

// Camera
float angleX = 45, angleY = -45, posX=0, posY=-1, posZ = 4, zoom = 1;

void monitorResolution(int *w, int *h){
  int count;
  const GLFWvidmode* modes = glfwGetVideoModes(glfwGetPrimaryMonitor(), &count);

  int maxWidth = 0; int maxHeight = 0;
  for(int i = 0; i < count; i++){
    if(modes[i].width > maxWidth)
      maxWidth = modes[i].width;
    if(modes[i].height > maxHeight)
      maxHeight = modes[i].height;
  }

  *w = maxWidth  / SCREEN_SIZE_CUT;
  *h = maxHeight / SCREEN_SIZE_CUT;
}

void openglInit(){
	glfwInit();
	monitorResolution(&SCREEN_WIDTH, &SCREEN_HEIGHT);
	glfwWindowHint(GLFW_SAMPLES, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwSwapInterval(1);
	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Atowers II", NULL, NULL);
 	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
 	
	glfwMakeContextCurrent(window);
	glfwSetCursorPos(window, SCREEN_WIDTH/2, SCREEN_HEIGHT/2);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

  glfwGetWindowSize(window, &windowWidth, &windowHeight);

	if (glewInit() != GLEW_OK) {
		fprintf(stdin, "Failed to initialize GLEW\n");
		exit(1);
	}

  glEnable(GL_TEXTURE_2D);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_DEPTH_TEST);
}

void updateCamera(){
  if(glfwGetKey(window, GLFW_KEY_LEFT))
    angleX -= 1;
  if(glfwGetKey(window, GLFW_KEY_RIGHT))
    angleX += 1;
  if(glfwGetKey(window, GLFW_KEY_UP) && angleY > -80)
    angleY -= 1;
  if(glfwGetKey(window, GLFW_KEY_DOWN) && angleY < -30)
    angleY += 1;
  if(glfwGetKey(window, GLFW_KEY_W))
    posY -= 0.01;
  if(glfwGetKey(window, GLFW_KEY_S))
    posY += 0.01;
  if(glfwGetKey(window, GLFW_KEY_A))
    posX += 0.01;
  if(glfwGetKey(window, GLFW_KEY_D))
    posX -= 0.01;
  if(glfwGetKey(window, GLFW_KEY_P) && zoom < 2)
    zoom *= 1.04;
  if(glfwGetKey(window, GLFW_KEY_O) && zoom > 0.5)
    zoom /= 1.04;
}

void mouseCallback(GLFWwindow* window, int button, int action, int mods){
  if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    printf("right click\n");
}

int main(int argc, char **argv){
	openglInit();	
	
  int sideTexture = Loader::loadPng("side.png");
  int topTexture =  Loader::loadPng("top.png");
  Cube cube(topTexture, sideTexture);
  vec3 dir;

  last_tick = glfwGetTime();
  glfwSetMouseButtonCallback(window, mouseCallback);
	
  do{
    float now = glfwGetTime();
    float elapsed = now - last_tick;
    last_tick = now;

    glViewport(0, 0, windowWidth, windowWidth);
    glColor4f(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    updateCamera();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0*zoom, 1.0*zoom, -1.0*zoom, 1.0*zoom, -10.0, 10.0);
    //gluPerspective(65.0, windowWidth / (float) windowHeight, 0.01, 100.0);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
      glTranslatef(posX, posY, posZ);
      glRotatef(angleY, 1, 0, 0);
      glRotatef(angleX, 0, 0, 1);
      glScalef(0.1f,0.1f,0.1f);

      for(int i=0; i<12; i++)
        for(int j=0; j<8; j++){
          glPushMatrix();
            glTranslatef(i, j, i == 11 || j == 7 || i == 0 || j == 0 ? -1 : 0.1*(i+j)-1 );
            cube.draw();
          glPopMatrix();
        }

      glFlush();
    glPopMatrix();

		glfwSwapBuffers(window);
		glfwPollEvents();

	} while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	glfwTerminate();

	return 0;
}
