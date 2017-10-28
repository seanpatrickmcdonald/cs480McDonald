#ifndef PHYSICS_OBJECT_H_
#define PHYSICS_OBJECT_H_


#include "graphics_headers.h"
#include "object.h"


class PhysicsObject: public Object
{
public:
  PhysicsObject();
  PhysicsObject(std::string objFilename, PhysicsManager *physics_manager);
  ~PhysicsObject();

};









#endif
