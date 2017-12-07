#include "character.h"

Character::Character()
{

}

Character::Character(std::string objFilename, std::string texFilename, PhysicsManager *physics_manager, btVector3 characterOrigin)
: Object(objFilename, texFilename, characterOrigin)
{
  	convexShape = new btCapsuleShape(0.25, 0.25);
  	ghostObject = new btPairCachingGhostObject();
	ghostObject->setWorldTransform(btTransform(btQuaternion(0,0,0,0.5),characterOrigin));
	physics_manager->dynamicsWorld->getPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
	ghostObject->setCollisionShape(convexShape);
	ghostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
	controller = new btKinematicCharacterController(ghostObject,convexShape,0.5);
	physics_manager->dynamicsWorld->addCollisionObject(ghostObject,btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter|btBroadphaseProxy::DefaultFilter);
	physics_manager->dynamicsWorld->addAction(controller);
}

Character::~Character()
{

}

