#include "World.hpp"
#include <vector>
#include "SDL/SDL_opengl.h"
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real_distribution.hpp>
#include <boost/foreach.hpp>
#include <time.h>
#include "Robot.hpp"

using namespace std;

class World::Private {
public:
  World& world;
  btDefaultCollisionConfiguration collisionConfiguration;
  btCollisionDispatcher dispatcher;
  btDbvtBroadphase broadphase;
  btSequentialImpulseConstraintSolver solver;
  btDiscreteDynamicsWorld dynamicsWorld;
  vector<btVector3> obstacleLocations;
  double obstacleSize;
  Robot* robot;
  Private(World& world, double obstacleSize, btVector3 bounds) : world(world),
								 obstacleSize(obstacleSize),
								 dispatcher(&collisionConfiguration), 
								 dynamicsWorld(&dispatcher, &broadphase, &solver, &collisionConfiguration) {
    
  }
};

class DebugDraw: public btIDebugDraw
{
private:
  int debugMode;
public:
  DebugDraw() :
    debugMode(1)
  {
  }

  void drawLine(const btVector3& from, const btVector3& to,
		const btVector3& color)
  {
    //cout << color.getX() << " " << color.getY() << " " << color.getZ() << endl;
    //cout << "    " << from.getX() << " " << from.getY() << " " << from.getZ() << endl;
    //cout << "    " << to.getX() << " " << to.getY() << " " << to.getZ() << endl;
    glBegin(GL_LINES);
    //glLineWidth(100);
    glColor3f(color.getX(), color.getY(), color.getZ());
    glVertex3f(from.getX(), from.getY(), from.getZ());
    glVertex3f(to.getX(), to.getY(), to.getZ());
    glEnd();
  }
  void drawContactPoint(const btVector3& from, const btVector3& to,
			btScalar q, int w, const btVector3& color)
  {
  }
  void reportErrorWarning(const char* error)
  {
    cout << error << endl;
  }
  void draw3dText(const btVector3& q, const char* w)
  {
  }
  void setDebugMode(int q)
  {
    debugMode = q;
  }
  int getDebugMode() const
  {
    return debugMode;
  }
};

World::World(btVector3 bounds, int numObstacles, double obstacleSize) : priv(new Private(*this, obstacleSize, bounds)) {
  priv->dynamicsWorld.setGravity(btVector3(0, 0, 0));
  priv->dynamicsWorld.setDebugDrawer(new DebugDraw());
  random::mt19937 generator(time(NULL));
  random::uniform_real_distribution<> widthDistribution(0, bounds.getX());
  random::uniform_real_distribution<> heightDistribution(0, bounds.getY());
  
  btCompoundShape* worldShape = new btCompoundShape();

  for(int i = 0;i < numObstacles;i++) {
    btBoxShape* box = new btBoxShape(btVector3(obstacleSize/2, obstacleSize/2, .1));

    btTransform transform(btTransform::getIdentity());
    transform.setOrigin(btVector3(widthDistribution(generator), heightDistribution(generator), 0));
    priv->obstacleLocations.push_back(btVector3(transform.getOrigin()));
    
    worldShape->addChildShape(transform, box);
  }

  btTransform transform(btTransform::getIdentity());
  transform.setOrigin(btVector3(-1, bounds.getY()/2, 0));
  worldShape->addChildShape(transform, new btBoxShape(btVector3(1, bounds.getY(), .1)));
  transform.setOrigin(btVector3(1 + bounds.getX(), bounds.getY()/2, 0));
  worldShape->addChildShape(transform, new btBoxShape(btVector3(1, bounds.getY(), .1)));
  transform.setOrigin(btVector3(bounds.getX()/2, -1, 0));
  worldShape->addChildShape(transform, new btBoxShape(btVector3(bounds.getX(), 1, .1)));
  transform.setOrigin(btVector3(bounds.getX()/2, 1 + bounds.getY(), 0));
  worldShape->addChildShape(transform, new btBoxShape(btVector3(bounds.getY(), 1, .1)));

  priv->robot = new Robot(priv->dynamicsWorld, bounds, priv->obstacleLocations, obstacleSize);

  btRigidBody::btRigidBodyConstructionInfo constructionInfo(0, new btDefaultMotionState(btTransform::getIdentity()), worldShape);
  priv->dynamicsWorld.addRigidBody(new btRigidBody(constructionInfo));
}

void World::update(double delta) {
  priv->dynamicsWorld.stepSimulation(delta);
  priv->robot->update();
}

void World::render() {
  priv->dynamicsWorld.debugDrawWorld();
  priv->robot->render();
  /*glBegin(GL_QUADS);
  glColor3f(1, 0, 0);
  BOOST_FOREACH(btVector3 obstacle, priv->obstacleLocations) {
    glVertex2f(obstacle.getX()-priv->obstacleSize/2.1, obstacle.getY()-priv->obstacleSize/2.1);
    glVertex2f(obstacle.getX()+priv->obstacleSize/2.1, obstacle.getY()-priv->obstacleSize/2.1);
    glVertex2f(obstacle.getX()+priv->obstacleSize/2.1, obstacle.getY()+priv->obstacleSize/2.1);
    glVertex2f(obstacle.getX()-priv->obstacleSize/2.1, obstacle.getY()+priv->obstacleSize/2.1);
  }
  glEnd();*/
}
