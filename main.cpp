
#include <iostream>
#include "main.hpp"
#include "shaders.hpp"
#include "terrain.hpp"
#include "clickable.hpp"
#include "mapgenerator.hpp"
#include <glm/glm.hpp>
#include <string.h>

#define SCREEN_SIZE_CUT 1.2f

using namespace std;
using namespace glm;

int frame = 0, windowWidth, windowHeight;
double timebase = 0, tm = 0, last_tick;
double fps;

// Camera
float angleX = 45, angleY = -45, posX=0, posY=-1, posZ = 4, zoom = 1;
int mouseX, mouseY;
GLuint frameBuffer = 0;

bool keyboard[1024];

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
  if(glfwGetKey(window, GLFW_KEY_W)){
    posX -= 0.01 * cos(radians(-angleX+90));
    posY -= 0.01 * sin(radians(-angleX+90));
  }
  if(glfwGetKey(window, GLFW_KEY_S)){
    posX += 0.01 * cos(radians(-angleX+90));
    posY += 0.01 * sin(radians(-angleX+90));
  }
  if(glfwGetKey(window, GLFW_KEY_A)){
    posX += 0.01 * cos(radians(-angleX));
    posY += 0.01 * sin(radians(-angleX));
  }
  if(glfwGetKey(window, GLFW_KEY_D)){
    posX -= 0.01 * cos(radians(-angleX));
    posY -= 0.01 * sin(radians(-angleX));
  }
  if(glfwGetKey(window, GLFW_KEY_P) && zoom < 10)
    zoom *= 1.04;
  if(glfwGetKey(window, GLFW_KEY_O) && zoom > 0.5)
    zoom /= 1.04;
}

int colorId = 0;

void mouseCallback(GLFWwindow* window, int button, int action, int mods){
  if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS){
    printf("right click %d\n", colorId);

  }
}

void cursorMoveCallback(GLFWwindow* window, double xpos, double ypos){
  mouseX = xpos;
  mouseY = ypos;
}

void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods){
  // TODO: Use keyboard callback to remove lag
  /*if(action != 2)
    printf("%d %d\n", key, action);*/
}

int main(int argc, char **argv){
  openglInit();
  memset(keyboard, false, sizeof(keyboard));
  printf("%d %d\n", GLFW_PRESS, GLFW_RELEASE);
    
  int mapHeight = 20, mapWidth = 20;
  Cube*** terrain = new Cube**[mapHeight];

  //Cube cube(topTexture, sideTexture, vec3());
  vec3 dir;

  last_tick = glfwGetTime();
  glfwSetMouseButtonCallback(window, mouseCallback);
  glfwSetCursorPosCallback(window, cursorMoveCallback);
  glfwSetKeyCallback(window, keyboardCallback);

  /*glGenFramebuffers(1, &frameBuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
  GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
  glDrawBuffers(1, DrawBuffers);*/
  double** heights;
  MapGenerator a;
  heights = a.generateMap(2, mapHeight, mapWidth);
  for(int i = 0; i < mapHeight; i++){
    terrain[i] = new Cube*[mapWidth];
    for(int j = 0; j < mapWidth; j++)
      terrain[i][j] = new Grass(vec3(i, j, heights[i][j]));
  }
  
  float totalTime = 0.0f;
  int frameCount = 0;

  GLint redBits, greenBits, blueBits;

  glGetIntegerv (GL_RED_BITS, &redBits);
  glGetIntegerv (GL_GREEN_BITS, &greenBits);
  glGetIntegerv (GL_BLUE_BITS, &blueBits);

  do{
    float now = glfwGetTime();
    float elapsed = now - last_tick;
    last_tick = now;

    totalTime += elapsed;
    frameCount++;
    if(totalTime >= 1.0){
      printf("\033[A\033[2Kfps: %d\n", frameCount);
      totalTime -= 1.0;
      frameCount = 0;
    }

    glViewport(0, 0, windowWidth, windowHeight);
    glColor4f(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    updateCamera();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0*zoom, 1.0*zoom, -1.0*zoom*((float)windowHeight)/windowWidth, 1.0*zoom*((float)windowHeight)/windowWidth, -10.0, 10.0);

    //glCullFace(GL_FRONT);
    //glEnable(GL_CULL_FACE);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
      glRotatef(angleY, 1, 0, 0);
      glRotatef(angleX, 0, 0, 1);
      glTranslatef(posX, posY, 0);
      glScalef(0.1f,0.1f,0.1f);


      glDisable (GL_BLEND);
      glDisable (GL_DITHER);
      glDisable (GL_FOG);
      glDisable (GL_LIGHTING);
      glDisable (GL_TEXTURE_1D);
      glDisable (GL_TEXTURE_2D);
      glDisable (GL_TEXTURE_3D);
      glShadeModel (GL_FLAT);

      unsigned int id = 1;
      for(int i = 0; i < mapHeight; i++)
        for(int j = 0; j < mapWidth; j++){
          glPushMatrix();
            terrain[i][j]->drawTriangles(id++);
          glPopMatrix();
        }
      glFlush();
      glFinish();

      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
      GLubyte data[4];
      glReadPixels(mouseX, windowHeight - mouseY, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, (&data));
      colorId = data[0] + data[1] * 256 + data[2] * 65536;
      
      glEnable(GL_TEXTURE_2D);
      glEnable(GL_BLEND);
      glShadeModel(GL_SMOOTH);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

      for(int i = 0; i < mapWidth; i++)
        for(int j = 0; j < mapHeight; j++){
          glPushMatrix();
            terrain[i][j]->draw();
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
