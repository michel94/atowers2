#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <iostream>
#include <math.h>

#include <GL/glew.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>

#ifdef __unix
	#define CONSOLE_FPS 1
#endif

#define DISPLAY_FPS 1

using namespace std;

int SCREEN_WIDTH;
int SCREEN_HEIGHT;

void monitor_resolution(int* w, int* h);

GLFWwindow* window;

#endif
