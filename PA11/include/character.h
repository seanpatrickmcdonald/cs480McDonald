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

  btConvexShape *convexShape;
  btPairCachingGhostObject *ghostObject;
  btKinematicCharacterController *controller;

  Mesh *m_mesh;

  uint scale;
  uint physicsIndex;

};


#endif