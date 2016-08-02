#include "engine.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <map>

using namespace glm;

Engine::Engine(int width, int height){
  SCREEN_WIDTH = width;
  SCREEN_HEIGHT = height;
  openglInit();

  last_tick = glfwGetTime();
  glfwSetMouseButtonCallback(window, mouseCallback);
  glfwSetCursorPosCallback(window, cursorMoveCallback);
  glfwSetKeyCallback(window, keyboardCallback);

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

}

void Engine::run(){
  float totalTime = 0.0f;
  int frameCount = 0;
  
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

    mat4 MVP;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    MVP = ortho(-1.0*zoom, 1.0*zoom, -1.0*zoom*((float)windowHeight)/windowWidth, 1.0*zoom*((float)windowHeight)/windowWidth, -10.0, 10.0);

    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
      MVP = rotate(MVP, radians(angleY), vec3(1,0,0));
      MVP = rotate(MVP, radians(angleX), vec3(0,0,1));
      MVP = translate(MVP, vec3(posX,posY,0));
      MVP = scale(MVP, vec3(0.1f,0.1f,0.1f));
      /*
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
      */
      glEnable(GL_TEXTURE_2D);
      glEnable(GL_BLEND);
      glShadeModel(GL_SMOOTH);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
            
      glUseProgram(program);

      for(int i=0; i<(signed)drawableObjects.size(); i++){
        drawableObjects[i]->draw(&MVP);
      }
      glFlush();

    glPopMatrix();

    glfwSwapBuffers(window);
    glfwPollEvents();

  } while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
       glfwWindowShouldClose(window) == 0 );

}


void Engine::openglInit(){
  Engine* engine = this;
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

  glfwSetWindowUserPointer(window, engine);
  glEnable(GL_TEXTURE_2D);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_DEPTH_TEST);
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

int Engine::getColorId(){
  return colorId;
}

void Engine::mouseCallback(GLFWwindow* window, int button, int action, int mods){
  Engine* engine = (Engine*) glfwGetWindowUserPointer(window);
  if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
    Clickable* object = engine->getCurrentClickable();
    if(object != NULL)
      object->onClick();
  }

}

Clickable* Engine::getCurrentClickable(){
  if(colorId > 0 && colorId < (signed) clickableObjects.size())
    return clickableObjects[colorId];
  else
    return NULL;
}

void Engine::updateCamera(){
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
