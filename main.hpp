#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <iostream>

#ifdef __unix
	#define CONSOLE_FPS 1
#endif

#define DISPLAY_FPS 1

void monitor_resolution(int* w, int* h);

#endif
