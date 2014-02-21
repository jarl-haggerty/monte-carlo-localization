#ifndef ROBOT_HPP
#define ROBOT_HPP

#include "World.hpp"
#include <vector>

#define RESOLUTION  36
#define SAMPLES 2000

using namespace std;

class Robot {
public:
  Robot(btDiscreteDynamicsWorld&, btVector3, vector<btVector3>&, double);
  void update();
  void render();
private:
  class Private;
  shared_ptr<Private> priv;
};

#endif
