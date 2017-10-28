#include "physics_object.h"

PhysicsObject::PhysicsObject()
{

}

PhysicsObject::~PhysicsObject()
{

}


PhysicsObject::PhysicsObject(PhysicsObjectStruct objStruct, PhysicsManager *physics_manager)
: Object(objStruct.objFilename)
{  
  btCollisionShape *collisionShape = new btSphereShape (btScalar(1.0f));
  btVector3 origin     = btVector3(0,0,0);
  btScalar mass        = btScalar(1);
  btScalar restitution = btScalar(1.0f);
  btVector3 inertia    = btVector3(0.0f, 0.0f, 0.0f);
  bool kinematic       = false;

  physics_manager->AddRigidBody(collisionShape, origin, mass, restitution, inertia, kinematic);
}

