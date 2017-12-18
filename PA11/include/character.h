#ifndef CHARACTER_H_
#define CHARACTER_H_

#include "graphics_headers.h"
#include "object.h"
#include "Mesh.h"

class Character
{
public:
  Character();
  Character(std::string objFilename, std::string texFilename, PhysicsManager *physics_manager, btVector3 characterOrigin = btVector3(0.0, 0.0, 0.0));
  Character(std::string objFilename, std::string texFilename, PhysicsManager *physics_manager, bool asdf);
  ~Character();

  void Render(Shader*);
  void ShadowRender(GLuint);

  static btVector3 character_origin;

  void UpdateDynamicCollisions();

  btConvexShape *convexShape;
  btPairCachingGhostObject *ghostObject;
  btKinematicCharacterController *controller;

  Mesh *m_mesh;
  PhysicsManager *m_physics_manager;
  btRigidBody* m_kinematic_body;

  uint scale;
  uint physicsIndex;

};


#endif