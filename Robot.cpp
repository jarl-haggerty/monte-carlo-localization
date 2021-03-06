#include "Robot.hpp"
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real_distribution.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/discrete_distribution.hpp>
#include <boost/random/normal_distribution.hpp>
#include <iostream>
#include <boost/foreach.hpp>
#include "SDL/SDL_opengl.h"
#include <float.h>

class Sample {
public:
  btVector3 position, direction;
  btMatrix3x3 rotation;
  vector<double> data;
  Sample(btVector3 position, int resolution) : position(position), rotation(cos(SIMD_2_PI/resolution), sin(SIMD_2_PI/resolution), 0,
									    -sin(SIMD_2_PI/resolution), cos(SIMD_2_PI/resolution), 0,
									    0, 0, 0) {} 
  void render() {
    btVector3 direction = -this->direction;
    glBegin(GL_LINES);
    glColor3f(.2, .2, .2);
    BOOST_FOREACH(double distance, data) {
      glVertex2f(position.getX() + direction.getX()*1.12, position.getY() + direction.getY()*1.12);
      glVertex2f(position.getX() + direction.getX()*distance, position.getY() + direction.getY()*distance);
      direction = rotation*direction;
    }
    glEnd();
  }
};

class Robot::Private {
public:
  Robot& robot;
  btDiscreteDynamicsWorld& world;
  btRigidBody* body;
  Sample reading;
  //vector<Sample>* samples;
  vector<btVector3>* samples;
  int i, direction, sampleCount, resolution;
  double scanRadius;
  random::mt19937 generator;
  random::uniform_int_distribution<> directionDistribution;
  Private(Robot& robot, btDiscreteDynamicsWorld& world) : robot(robot), world(world), reading(btVector3(0, 0, 0), 1),
							  generator(time(NULL)), 
							  directionDistribution(-1, 1), i(0), scanRadius(100), 
							  direction(0), resolution(RESOLUTION), sampleCount(SAMPLES) {
  }
};

Robot::Robot(btDiscreteDynamicsWorld& world, btVector3 bounds, 
	       vector<btVector3>& obstacleLocations, double obstacleSize) : priv(new Private(*this, world)) {
  random::uniform_real_distribution<> rotationDistribution(0, SIMD_2_PI);
  random::uniform_real_distribution<> xDistribution(2, bounds.getX()-2);
  random::uniform_real_distribution<> yDistribution(2, bounds.getY()-2);
  
  bool hit = true;
  btBoxShape* shape = new btBoxShape(btVector3(1, .5, .1));
  double minDistance = sqrt(2*pow(obstacleSize/2+1, 2));
  double rotation = rotationDistribution(priv->generator);
  btMatrix3x3 rotationMatrix(cos(rotation), sin(rotation), 0, 
			     -sin(rotation), cos(rotation), 0, 
			     0, 0, 0);
  btVector3 position;
  while(hit) {
    position.setValue(xDistribution(priv->generator), yDistribution(priv->generator), 0);
    BOOST_FOREACH(btVector3 obstacle, obstacleLocations) {
      hit = (obstacle-position).length() < minDistance;
      if(hit) {
	break;
      }
    }
  }
  priv->reading = Sample(position, priv->resolution);

  btRigidBody::btRigidBodyConstructionInfo constructionInfo(1, new btDefaultMotionState(btTransform(rotationMatrix, position)), 
							    shape, btVector3(0, 0, 1));
  priv->body = new btRigidBody(constructionInfo);
  world.addRigidBody(priv->body);
  priv->body->setActivationState(DISABLE_DEACTIVATION);

  priv->samples = new vector<btVector3>();
  for(int i = 0;i < priv->sampleCount;i++) {
    priv->samples->push_back(btVector3(xDistribution(priv->generator), yDistribution(priv->generator), 0));
    //for(int u = 0;u < priv->resolution;u++) {
    //  priv->samples->at(i).data.push_back(0);
    //}
  }

  cout << hit << endl;
}

void Robot::update() {
  priv->body->applyTorqueImpulse(-priv->body->getAngularVelocity());
  priv->body->applyCentralImpulse(-priv->body->getLinearVelocity());
  btMatrix3x3 rotation = priv->body->getWorldTransform().getBasis();
  btVector3 forward = rotation*btVector3(1, 0, 0);
  priv->body->applyCentralImpulse(3*forward);

  if(priv->i == 0) {
    priv->direction = priv->directionDistribution(priv->generator);
  }
  priv->i = (priv->i + 1) % 60;
  priv->body->applyTorqueImpulse(btVector3(0, 0, priv->direction));

  btVector3 direction = -forward;
  rotation = btMatrix3x3(cos(SIMD_2_PI/priv->resolution), sin(SIMD_2_PI/priv->resolution), 0,
			 -sin(SIMD_2_PI/priv->resolution), cos(SIMD_2_PI/priv->resolution), 0,
			 0, 0, 0);
  priv->reading.position = priv->body->getWorldTransform().getOrigin();
  priv->reading.direction = forward;
  priv->reading.data.clear();
  btCollisionWorld::ClosestRayResultCallback callback(btVector3(0, 0, 0), btVector3(0, 0, 0));
  for(int i = 0;i < priv->resolution;i++) {
    callback.m_rayFromWorld = priv->reading.position;
    callback.m_rayToWorld = callback.m_rayFromWorld + direction*priv->scanRadius;
    callback.m_closestHitFraction = 1;
    callback.m_collisionObject = NULL;
    priv->world.rayTest(callback.m_rayFromWorld, callback.m_rayToWorld, callback);
    if(callback.hasHit()) {
      priv->reading.data.push_back(priv->scanRadius*callback.m_closestHitFraction);
    } else {
      priv->reading.data.push_back(priv->scanRadius);
    }
    direction = rotation*direction;
  }

  vector<double> odds;
  double difference, maximum = 0;  
  BOOST_FOREACH(btVector3 sample, *priv->samples) {
    direction = -forward;
    difference = 0;
    for(int i = 0;i < priv->resolution;i++) {
      callback.m_rayFromWorld = sample;
      callback.m_rayToWorld = callback.m_rayFromWorld + direction*priv->scanRadius;
      callback.m_closestHitFraction = 1;
      callback.m_collisionObject = NULL;
      priv->world.rayTest(callback.m_rayFromWorld, callback.m_rayToWorld, callback);
      if(callback.hasHit()) {
	difference += abs(priv->reading.data[i] - priv->scanRadius*callback.m_closestHitFraction);
      } else {
	difference += abs(priv->reading.data[i] - priv->scanRadius);
      }
      direction = rotation*direction;
    }
    odds.push_back(difference);
    if(difference > maximum) {
      maximum = difference;
    }
  }

  for(int i = 0;i < priv->samples->size();i++) {
    odds[i] = maximum-odds[i] + DBL_EPSILON;
  }

  vector<btVector3>* oldSamples = priv->samples;
  priv->samples = new vector<btVector3>();
  random::discrete_distribution<> dist(odds);
  random::normal_distribution<> jitter(0, .2);
  btVector3 estimate(0, 0, 0);
  for(int i = 0;i < max<int>(10, oldSamples->size()-oldSamples->size()/10);i++) {
    priv->samples->push_back(oldSamples->at(dist(priv->generator)));
    priv->samples->back() += btVector3(jitter(priv->generator), jitter(priv->generator), 0);
    estimate += priv->samples->back();
  }
  estimate /= priv->samples->size();
  cout << (priv->reading.position-estimate).length() << " " << priv->samples->size() << endl;
}

void Robot::render() {
  priv->reading.render();

  glBegin(GL_POINTS);
  glColor3f(0, 1, 1);
  BOOST_FOREACH(btVector3 sample, *priv->samples) {
    //    cout << sample.position.getX() << " " << sample.position.getY();
    glVertex2f(sample.getX(), sample.getY());
  }
  glEnd();
}
