#include <algorithm>
#include <World.hpp>
#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"
#include "btBulletDynamicsCommon.h"

using namespace std;

int main(int argc, char** argv) {
  btVector3 bounds(100, 100, 0);
  int numObstacles = 10, numSamples = 10, width = 600, height = 600;
  SDL_Surface* window;
  World world(bounds, numObstacles, 20);
  //Robot robot(world);
  //Localizer localizer(world, robot, numSamples);
  //Display display(world, robot, localizer);

  SDL_Init(SDL_INIT_EVERYTHING);
  window = SDL_SetVideoMode(width, height, 32, SDL_OPENGL);
  SDL_WM_SetCaption("Monte Carlo Localization", NULL);
  
  glViewport(0, 0, width, height);
  
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_SMOOTH);

  double period = 1.0/60;
  int lastTime = SDL_GetTicks();
  bool running = true;
  while(running) {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
      switch(event.type) {
      case SDL_QUIT:
	running = false;
	break;
      }
    }

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, bounds.getX(), bounds.getY(), 0, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //robot.update(period);
    world.render();
    //localizer.update(period);
    //display.update();

    SDL_GL_SwapBuffers();

    world.update(period);

    int time = SDL_GetTicks();
    int delay = 1000 * period - (time - lastTime);
    lastTime = time;
    SDL_Delay(min<int>(max<int>(delay, 0), 1000));
  }

  return 0;
}
