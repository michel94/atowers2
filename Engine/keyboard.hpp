#include <GLFW/glfw3.h>

void pollEvents();
void initKeyboard(GLFWwindow* w);
int getKey(GLFWwindow* w, int scancode);
void setKey(int scancode, bool b);
void setCursorPosCallback(GLFWwindow* window, GLFWcursorposfun cursorMoveCallback);
void setMouseButtonCallback(GLFWwindow* window, GLFWmousebuttonfun mouseButtonCallback);
void setScrollCallback(GLFWwindow* window, GLFWscrollfun callback);

