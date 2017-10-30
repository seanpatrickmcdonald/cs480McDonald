#include "physics_object.h"

PhysicsObject::PhysicsObject()
{

}

PhysicsObject::~PhysicsObject()
{

}


PhysicsObject::PhysicsObject(PhysicsObjectStruct objStruct, PhysicsManager *physics_manager)
: Object(objStruct.objFilename, objStruct.texFilename)
{  
  btCollisionShape *collisionShape;
    

  btVector3 Yup(0.0f, 1.0f, 0.0f);
/*

    Note: Replace the Constants with parameters passed from json to struct, etc. This is for primitives

*/
  if (objStruct.primitiveType == "sphere")
      collisionShape = new btSphereShape(btScalar(0.25f));
  else if (objStruct.primitiveType == "cylinder")
      collisionShape = new btCylinderShape(btVector3(0.25f, 0.5f, 0.5f));
  else if (objStruct.primitiveType == "box")
      collisionShape = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));
  else if (objStruct.primitiveType == "plane")
      collisionShape = new btStaticPlaneShape(Yup, 0);

  

  /*
    Replace this cout statement with triangulate code
  */
  else
      std::cout << "No primitive type for object name: " << objStruct.objName << std::endl;


  btVector3 origin     = objStruct.origin;
  btScalar mass        = btScalar(objStruct.mass);
  btScalar restitution = objStruct.restitution;
  btVector3 inertia    = btVector3(0.0f, 0.0f, 0.0f);
  bool kinematic       = false;

  physics_manager->AddRigidBody(collisionShape, origin, mass, restitution, inertia, kinematic);
}

