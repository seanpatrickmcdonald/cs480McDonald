#ifndef PHYSICS_OBJECT_H_
#define PHYSICS_OBJECT_H_


#include "graphics_headers.h"
#include "object.h"

struct PhysicsObjectStruct;

class PhysicsObject: public Object
{
public:
  PhysicsObject();
  ~PhysicsObject();
  PhysicsObject(PhysicsObjectStruct objStruct, PhysicsManager *physics_manager);
};

struct PhysicsObjectStruct
{
  std::string objFilename;
  std::string texFilename;
  std::string objName;
  btVector3 origin;
  btScalar mass;
  btScalar restitution;
  btVector3 inertia;
  bool kinematic;
  std::string primitiveType;
};









#endif
