#ifndef WORLD_HPP
#define WORLD_HPP

#include <boost/shared_ptr.hpp>
#include "btBulletDynamicsCommon.h"

using namespace boost;

class World {
public:
  World(btVector3, int, double);
  void update(double);
  void render();
private:
  class Private;
  shared_ptr<Private> priv;
};

#endif
