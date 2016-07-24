
#include <iostream>
#include "main.hpp"
#include "loader.hpp"
#include "shaders.hpp"

#define SCREEN_SIZE_CUT 1.2f

using namespace std;

int frame = 0, window_width, window_height;
double timebase = 0, tm = 0, last_tick;
double fps;

void opengl_init(){
	glfwInit();
	monitor_resolution(&SCREEN_WIDTH, &SCREEN_HEIGHT);
	glfwWindowHint(GLFW_SAMPLES, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwSwapInterval(0);
	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Atowers 2", NULL, NULL);
 	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
 	
	glfwMakeContextCurrent(window);
	glfwSetCursorPos(window, SCREEN_WIDTH/2, SCREEN_HEIGHT/2);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

  glfwGetWindowSize(window, &window_width, &window_height);

	if (glewInit() != GLEW_OK) {
		fprintf(stdin, "Failed to initialize GLEW\n");
		exit(1);
	}

  glEnable(GL_TEXTURE_2D);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_DEPTH_TEST);
}

void monitor_resolution(int *w, int *h){
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

void drawSquare(bool flip){
  float b = 0.0f, t = 1.0f;
  if(flip)
    swap(b, t);
  
  glBegin(GL_QUADS);
    glTexCoord2f(b, b);
    glVertex3f(0.0f, 0.0f, 0);
    glTexCoord2f(t, b);
    glVertex3f(1.0f, 0.0f, 0);
    glTexCoord2f(t, t);
    glVertex3f(1.0f, 1.0f, 0);
    glTexCoord2f(b, t);
    glVertex3f(0.0f, 1.0f, 0);
  glEnd();
  
}

void drawCube(int topTexture, int sideTexture){
  glBindTexture(GL_TEXTURE_2D, topTexture);
  drawSquare(false);
  glBindTexture(GL_TEXTURE_2D, sideTexture);
  glPushMatrix();
    glRotatef(270, 1, 0, 0);
    drawSquare(true);
    glRotatef(270, 0, 1, 0);
    drawSquare(true);
  glPopMatrix();
  glPushMatrix();
    glTranslatef(1, 1, 0);
    glRotatef(180, 0, 0, 1);
    glRotatef(270, 1, 0, 0);
    drawSquare(true);
    glRotatef(270, 0, 1, 0);
    drawSquare(true);
  glPopMatrix();
}

int main(int argc, char **argv){
	opengl_init();	
	
  int sideTexture = Loader::loadPng("side.png");
  int topTexture =  Loader::loadPng("top.png");

	last_tick = glfwGetTime();

  int x = 45, y = 0;
	
  do{
    glViewport(0, 0, window_width, window_height);
    glColor4f(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);

    
    //glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(65.0, window_width / (float) window_height, 0.01, 100.0);
    if(glfwGetKey(window, GLFW_KEY_LEFT))
      x -= 1;
    if(glfwGetKey(window, GLFW_KEY_RIGHT))
      x += 1;
    if(glfwGetKey(window, GLFW_KEY_UP))
      y += 1;
    if(glfwGetKey(window, GLFW_KEY_DOWN))
      y -= 1;
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
      glTranslatef(0, 0, -0.3);
      glRotatef(y, 1, 0, 0);
      glRotatef(x, 0, 0, 1);
      printf("%d\n", x);
      glScalef(0.1f,0.1f,0.1f);

      for(int i=0; i<12; i++)
        for(int j=0; j<8; j++){
          glPushMatrix();
            glTranslatef(i*1.05, j*1.05, i == 11 || j == 7 || i == 0 || j == 0 ? -1 : 0.1*(i+j)-1 );
            drawCube(topTexture, sideTexture);
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
