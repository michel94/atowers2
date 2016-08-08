#include "engine.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <map>

using namespace glm;

const float MAX_ZOOM = 5, MIN_ZOOM = 0.5;

Engine::Engine(int width, int height){
  SCREEN_WIDTH = width;
  SCREEN_HEIGHT = height;
  openglInit();

  last_tick = glfwGetTime();
  glfwSetMouseButtonCallback(window, mouseCallback);
  glfwSetCursorPosCallback(window, cursorMoveCallback);
  glfwSetKeyCallback(window, keyboardCallback);
  glfwSetScrollCallback(window, scrollCallback);
  glfwSetWindowSizeCallback(window, resizeCallback);

}

void Engine::loadMap(double** heights, int mapHeight, int mapWidth){
  program = loadShader("vertex.glsl", "frag.glsl", uniforms, {"MVP"});

  terrain = new Cube**[mapHeight];
  this->mapWidth = mapWidth;
  this->mapHeight = mapHeight;
  int id = 1;
  clickableObjects.resize(mapHeight * mapWidth + 1);
  for(int i = 0; i < mapHeight; i++){
    terrain[i] = new Cube*[mapWidth];
    for(int j = 0; j < mapWidth; j++){
      terrain[i][j] = new Grass(vec3(i, j, heights[i][j]));
      terrain[i][j]->MVPid = uniforms["MVP"];
      clickableObjects[id++] = terrain[i][j];
      drawableObjects.push_back(terrain[i][j]);
    }
  }
  glUseProgram(0);

}

const int gridSize = 32;

const int WIDTH = gridSize*32;
const int HEIGHT = gridSize*16;

void Engine::render2d(float elapsed){
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0f, 1.0f, 0.0f, 1.0f);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glUseProgram(0);
  // Menu rendered here
  glColor3f(0.8f, 0.7f, 0.0f);
  glBegin(GL_QUADS);
    glVertex2f(0.8f, 0.0f);
    glVertex2f(1.0f, 0.0f);
    glVertex2f(1.0f, 1.0f);
    glVertex2f(0.8f, 1.0f);
  glEnd();
}

void Engine::handleClick(mat4 MVP, int windowWidth, int windowHeight){
  if(!pendingClick)
    return;
  
  pendingClick = false;
  glLoadMatrixf(&MVP[0][0]);

  glDisable (GL_BLEND);
  glDisable (GL_DITHER);
  glDisable (GL_FOG);
  glDisable (GL_LIGHTING);
  glDisable (GL_TEXTURE_1D);
  glDisable (GL_TEXTURE_2D);
  glDisable (GL_TEXTURE_3D);
  glShadeModel (GL_FLAT);

  glUseProgram(0);
  unsigned int id = 1;
  for(int i=1; i<(signed)clickableObjects.size(); i++){
    glPushMatrix();
      clickableObjects[i]->drawTriangles(i);
    glPopMatrix();
  }
  glFlush();
  glFinish();

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  GLubyte data[4];
  glReadPixels(mouseX, windowHeight - mouseY, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, (&data));
  colorId = data[0] + data[1] * 256 + data[2] * 65536;
  Clickable* object = getCurrentClickable();
  if(object != NULL)
    object->onClick();

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
}

void Engine::render3d(float elapsed, int windowWidth, int windowHeight){  
  updateCamera(elapsed);
  
  mat4 MVP;
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  MVP = ortho(-1.0*zoom, 1.0*zoom, -1.0*zoom*((float)windowHeight)/windowWidth, 1.0*zoom*((float)windowHeight)/windowWidth, -10.0, 10.0);
  
  MVP = rotate(MVP, radians(angleY), vec3(1,0,0));
  MVP = rotate(MVP, radians(angleX), vec3(0,0,1));
  MVP = translate(MVP, vec3(posX,posY,0));
  MVP = scale(MVP, vec3(0.1f,0.1f,0.1f));

  handleClick(MVP, windowWidth, windowHeight);

  glUseProgram(program);

  for(int i=0; i<(signed)drawableObjects.size(); i++){
    drawableObjects[i]->draw(&MVP);
  }

}

void Engine::run(){
  do{
    float elapsed = elapsedTime();
    showFPS(elapsed);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glViewport(0, 0, 0.8*SCREEN_WIDTH, SCREEN_HEIGHT);

    glEnable(GL_DEPTH_TEST);
    render3d(elapsed, 0.8*SCREEN_WIDTH, SCREEN_HEIGHT);

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_DEPTH_TEST);
  
    render2d(elapsed);
  
    glFlush();
    glFinish();

    glfwSwapBuffers(window);
    glfwPollEvents();

} while( glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
     glfwWindowShouldClose(window) == 0 );

  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}

void Engine::openglInit(){
  
  glfwWindowHint(GLFW_SAMPLES, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwSwapInterval(1);
  window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Atowers II", NULL, NULL);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

  glfwMakeContextCurrent(window);
  glfwSetCursorPos(window, SCREEN_WIDTH/2, SCREEN_HEIGHT/2);

  glfwSetWindowUserPointer(window, this);

  if (glewInit() != GLEW_OK) {
    printf("Failed to initialize GLEW\n");
    exit(1);
  }
}

void Engine::resizeCallback(GLFWwindow* window, int width, int height){
  Engine* self = (Engine*) glfwGetWindowUserPointer(window);
  self->SCREEN_WIDTH = width;
  self->SCREEN_HEIGHT = height;
}

void Engine::cursorMoveCallback(GLFWwindow* window, double xpos, double ypos){
  Engine* self = (Engine*) glfwGetWindowUserPointer(window);
  self->mouseX = xpos;
  self->mouseY = ypos;
}

void Engine::keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods){
  // TODO: Use keyboard callback to remove lag
  /*if(action != 2)
    printf("%d %d\n", key, action);*/
}

void Engine::mouseCallback(GLFWwindow* window, int button, int action, int mods){
  Engine* engine = (Engine*) glfwGetWindowUserPointer(window);
  if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
    engine->pendingClick = true;
  }
}

void Engine::scrollCallback(GLFWwindow* window, double dx, double dy){
  Engine* engine = (Engine*) glfwGetWindowUserPointer(window);
  if( (dy < 0 && engine->zoom < MAX_ZOOM) || (dy > 0 && engine->zoom > MIN_ZOOM) )
    engine->zoom *= pow(1.04, -dy*3);
}

int Engine::getColorId(){
  return colorId;
}

Clickable* Engine::getCurrentClickable(){
  if(colorId > 0 && colorId < (signed) clickableObjects.size())
    return clickableObjects[colorId];
  else
    return NULL;
}

void Engine::updateCamera(float dt){
  if(glfwGetKey(window, GLFW_KEY_LEFT))
    angleX -= 50*dt;
  if(glfwGetKey(window, GLFW_KEY_RIGHT))
    angleX += 50*dt;
  if(glfwGetKey(window, GLFW_KEY_UP) && angleY > -80)
    angleY -= 50*dt;
  if(glfwGetKey(window, GLFW_KEY_DOWN) && angleY < -30)
    angleY += 50*dt;
  if(glfwGetKey(window, GLFW_KEY_W)){
    posX -= 0.5 * dt * cos(radians(-angleX+90));
    posY -= 0.5 * dt * sin(radians(-angleX+90));
  }
  if(glfwGetKey(window, GLFW_KEY_S)){
    posX += 0.5 * dt * cos(radians(-angleX+90));
    posY += 0.5 * dt * sin(radians(-angleX+90));
  }
  if(glfwGetKey(window, GLFW_KEY_A)){
    posX += 0.5 * dt * cos(radians(-angleX));
    posY += 0.5 * dt * sin(radians(-angleX));
  }
  if(glfwGetKey(window, GLFW_KEY_D)){
    posX -= 0.5 * dt * cos(radians(-angleX));
    posY -= 0.5 * dt * sin(radians(-angleX));
  }
  
}

void Engine::addObject(Drawable* obj){
  obj->MVPid = uniforms["MVP"];
  drawableObjects.push_back(obj);
  vec3& pos = obj->getPosition();
  Drawable* land = terrain[(int)pos.x][(int)pos.y];
  pos += vec3(0, 0, land->getPosition().z+1);
}

void Engine::addObject(Clickable* obj){
  addObject((Drawable*) obj);
  clickableObjects.push_back(obj);
}

float Engine::elapsedTime(){
  float now = glfwGetTime();
  float elapsed = now - last_tick;
  last_tick = now;

  return elapsed;
}

void Engine::showFPS(float elapsed){
  totalTime += elapsed;
  frameCount++;
  if(totalTime >= 1.0){
    printf("\033[A\033[2Kfps: %d\n", frameCount);
    totalTime -= 1.0;
    frameCount = 0;
  }
}
