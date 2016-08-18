#include "engine.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <map>

using namespace glm;

const float MAX_ZOOM = 5, MIN_ZOOM = 0.5;
const float BAR_PROP = 0.2;


Engine::Engine(GameLogic* game, int width, int height){
  SCREEN_WIDTH = width;
  SCREEN_HEIGHT = height;
  initialWidth = width;
  initialHeight = height;
  this->game = game;
  openglInit();

  last_tick = glfwGetTime();
  glfwSetMouseButtonCallback(window, mouseCallback);
  glfwSetCursorPosCallback(window, cursorMoveCallback);
  glfwSetKeyCallback(window, keyboardCallback);
  glfwSetScrollCallback(window, scrollCallback);
  glfwSetWindowSizeCallback(window, resizeCallback);
  glfwSetFramebufferSizeCallback(window, resizeCallback);

}

bool contains(vec2 orig, vec2 size, vec2 point){
  return  point.x >= orig.x && point.x <= orig.x + size.x && 
      point.y >= orig.y && point.y <= orig.y + size.y;
}

void Engine::loadMap(double** heights, int mapHeight, int mapWidth){
  program = loadShader("vertex.glsl", "frag.glsl", uniforms, {"MVP"});

  terrain = new Cube**[mapHeight];
  this->mapWidth = mapWidth;
  this->mapHeight = mapHeight;
  clickable3dObjects.resize(mapWidth * mapHeight + 1);
  int id = 1;
  for(int i = 0; i < mapHeight; i++){
    terrain[i] = new Cube*[mapWidth];
    for(int j = 0; j < mapWidth; j++){
      terrain[i][j] = new Grass(vec3(i, j, heights[i][j]));
      terrain[i][j]->MVPid = uniforms["MVP"];
      clickable3dObjects[id++] = terrain[i][j];
      drawable3dObjects.push_back(terrain[i][j]);
    }
  }
  glUseProgram(0);
  boardTexture = Loader::loadPng("menu/board.png");

}

void Engine::drawTriangles(Drawable* obj, unsigned int color){
  vector<Triangle> triangles = obj->getTriangles();
  vec3& pos = obj->getPosition();
  GLubyte r = color & 0x000000FF, g = (color & 0x0000FF00) >> 8,
          b = (color & 0x00FF0000) >> 16, a = (color & 0xFF000000) >> 24;
  
  glColor4ub(r, g, b, a);
  glTranslatef(pos.x, pos.y, pos.z);
  glBegin(GL_TRIANGLES);
  for(int i=0; i<(signed)triangles.size(); i++){
    glVertex3f(triangles[i].p1.x, triangles[i].p1.y, triangles[i].p1.z);
    glVertex3f(triangles[i].p2.x, triangles[i].p2.y, triangles[i].p2.z);
    glVertex3f(triangles[i].p3.x, triangles[i].p3.y, triangles[i].p3.z);
  }
  glEnd();
}

void Engine::render2d(float elapsed, int windowWidth, int windowHeight){
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0f, windowWidth, 0.0f, windowHeight);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glUseProgram(0);
  // Menu rendered here
  glEnable (GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, boardTexture);
  glColor3f(1.0f, 1.0f, 1.0f);
  int leftX = (1-BAR_PROP) * windowWidth;
  glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(leftX, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(windowWidth, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(windowWidth, windowHeight);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(leftX, windowHeight);
  glEnd();
  glScalef(windowWidth/initialWidth, windowHeight/initialHeight, 1.0f);
  for(int i=0; i<(signed)drawable2dObjects.size(); i++){
    drawable2dObjects[i]->draw(NULL);
  }

  handleClick2d(windowWidth, windowHeight);
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

  handleClick3d(MVP, windowWidth, windowHeight);

  glUseProgram(program);

  for(int i=0; i<(signed)drawable3dObjects.size(); i++){
    drawable3dObjects[i]->draw(&MVP);
  }

}

void Engine::handleClick2d(int windowWidth, int windowHeight){
  if(!pendingMove || !click2d)
    return;
  pendingMove = false;
  Drawable2d* over = NULL;
  for(int i=0; i<(signed)clickable2dObjects.size(); i++){
    Drawable2d* obj = clickable2dObjects[i];
    vec2 pos = vec2(obj->getPosition().x, obj->getPosition().y);
    vec2 size = obj->getSize();
    if(contains(pos, size, vec2(mouseX, windowHeight - mouseY))){
      over = obj;
    }
  }
  setOverObject(over);
}

void Engine::handleClick3d(mat4 MVP, int windowWidth, int windowHeight){
  if(!pendingMove || click2d)
    return;
  
  pendingMove = false;
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
  for(int i=1; i<(signed)clickable3dObjects.size(); i++){
    glPushMatrix();
      drawTriangles(clickable3dObjects[i], i);
    glPopMatrix();
  }
  glFlush();
  glFinish();

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  GLubyte data[4];
  if(mouseX < 0 || mouseX > windowWidth || windowHeight - mouseY < 0 || windowHeight - mouseY > windowHeight){
    printf("OUT\n");
    colorId = 0;
    setOverObject(getCurrentClickable());
  }else{
    glReadPixels(mouseX, windowHeight - mouseY, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, (&data));
    colorId = data[0] + data[1] * 256 + data[2] * 65536;
    setOverObject(getCurrentClickable());
  }
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
}

void Engine::run(){
  do{
    float elapsed = elapsedTime();
    showFPS(elapsed);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glViewport(0, 0, (1-BAR_PROP)*SCREEN_WIDTH, SCREEN_HEIGHT);

    glEnable(GL_DEPTH_TEST);
    render3d(elapsed, (1-BAR_PROP)*SCREEN_WIDTH, SCREEN_HEIGHT);

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_DEPTH_TEST);
    
    render2d(elapsed, SCREEN_WIDTH, SCREEN_HEIGHT);

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
  glfwInit();
  glfwWindowHint(GLFW_SAMPLES, 0);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwSwapInterval(1);
  window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Atowers II", NULL, NULL);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

  glfwMakeContextCurrent(window);
  glfwSetCursorPos(window, SCREEN_WIDTH/2, SCREEN_HEIGHT/2);

  glfwSetWindowUserPointer(window, this);
  glfwSetWindowAspectRatio(window, 16, 9);

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
  Engine* engine = (Engine*) glfwGetWindowUserPointer(window);
  if(engine->mouseX != xpos || engine->mouseY != ypos)
    engine->pendingMove = true;

  engine->mouseX = xpos;
  engine->mouseY = ypos;

  engine->click2d = engine->mouseX < (1-BAR_PROP) * engine->SCREEN_WIDTH ? false : true;
}

void Engine::keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods){
  // TODO: Use keyboard callback to remove lag
  /*if(action != 2)
    printf("%d %d\n", key, action);*/
}

void Engine::mouseCallback(GLFWwindow* window, int button, int action, int mods){
  Engine* engine = (Engine*) glfwGetWindowUserPointer(window);
  if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
    if(engine->getOverObject() != NULL){
      engine->getOverObject()->onClick(engine->getGame());
      engine->getGame()->onClick(engine->getOverObject());
    }
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

Drawable* Engine::getCurrentClickable(){
  if(colorId > 0 && colorId < (signed) clickable3dObjects.size())
    return clickable3dObjects[colorId];
  else
    return NULL;
}

bool Engine::checkCameraKeys(){
  for(int i=0; i < (signed) cameraKeys.size(); i++)
    if(glfwGetKey(window, cameraKeys[i]))
      return true;
  return false;
}

void Engine::updateCamera(float dt){
  if(checkCameraKeys())
    pendingMove = true;

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

void Engine::addObject3D(Drawable* obj){
  obj->engineData.is2d = false;

  obj->MVPid = uniforms["MVP"];
  drawable3dObjects.push_back(obj);
  vec3& pos = obj->getPosition();
  Drawable* land = terrain[(int)pos.x][(int)pos.y];
  pos += vec3(0, 0, land->getPosition().z+1);
}

double Engine::getTerrainHeight(int x, int y){
  return terrain[x][y]->getPosition().z+1;
}

void Engine::addObject2D(Drawable2d* obj){
  obj->engineData.is2d = true;

  drawable2dObjects.push_back(obj);
}

void Engine::makeClickable(Drawable* obj, bool clickable){
  if(obj->isClickable() == clickable)
    return;
  obj->setClickable(clickable);

  if(clickable){
    clickable3dObjects.push_back(obj);
  }else{
    remove(clickable3dObjects, obj);
  }
}

void Engine::makeClickable(Drawable2d* obj, bool clickable){
  if(obj->isClickable() == clickable)
    return;
  obj->setClickable(clickable);

  if(clickable){
    clickable2dObjects.push_back(obj);
  }else{
    remove(clickable2dObjects, obj);
  }
  
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

GameLogic* Engine::getGame(){
  return game;
}

GLFWwindow* Engine::getWindow(){
  return window;
}

/*
void Engine::setMouseDrawable(Drawable* obj){
  mouseCursor = obj;
}

void Engine::removeMouseDrawable(){
  mouseCursor = NULL;
}
*/

Drawable* Engine::getOverObject(){
  return overObj;
}

GameLogic* Engine::getGameObject(){
  return game;
}

void Engine::setOverObject(Drawable* obj){
  overObj = obj;
  if(game != NULL)
    if(overObj != NULL && !overObj->engineData.is2d)
      game->onOver(overObj);
}

