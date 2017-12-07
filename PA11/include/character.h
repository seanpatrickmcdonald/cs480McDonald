#ifndef CHARACTER_H_
#define CHARACTER_H_

#include "graphics_headers.h"
#include "object.h"

class Character : public Object
{
public:
  Character();
  Character(std::string objFilename, std::string texFilename, PhysicsManager *physics_manager, btVector3 characterOrigin = btVector3(0.0, 0.0, 0.0));
  ~Character();

  btConvexShape *convexShape;
  btPairCachingGhostObject *ghostObject;
  btKinematicCharacterController *controller;

};


#endif