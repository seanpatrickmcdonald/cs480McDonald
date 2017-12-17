#include "character.h"

Character::Character()
{

}

Character::Character(std::string objFilename, std::string texFilename, 
	PhysicsManager *physics_manager, btVector3 characterOrigin)
{
  	convexShape = new btCylinderShape( btVector3(0.25, 0.25, 0.25) );
  	ghostObject = new btPairCachingGhostObject();
	ghostObject->setWorldTransform(btTransform(btQuaternion(0, 0, 0),characterOrigin));
	physics_manager->dynamicsWorld->getPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
	ghostObject->setCollisionShape(convexShape);
	ghostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
	controller = new btKinematicCharacterController(ghostObject,convexShape,0.125);
	physics_manager->dynamicsWorld->addCollisionObject(ghostObject,btBroadphaseProxy::KinematicFilter, btBroadphaseProxy::StaticFilter|btBroadphaseProxy::DefaultFilter);
	physics_manager->dynamicsWorld->addAction(controller);

	m_mesh = new Mesh();
	m_mesh->LoadVertexData(objFilename);
}

Character::Character(std::string objFilename, std::string texFilename, PhysicsManager *physics_manager, bool asdf)
{   
    btVector3 Yup(0.0f, 1.0f, 0.0f);

    physicsIndex = physics_manager->GetNumObjects();
    //std::cout << "Character PhysicsIndex: " << physicsIndex << std::endl;

    btVector3 origin     = btVector3(0.0, 0.5, 0.0);
    btScalar mass        = btScalar(100);
    btScalar restitution = 0.1;
    btVector3 inertia    = btVector3(0.0f, 0.0f, 0.0f);
    bool kinematic = true;


  	convexShape = new btCapsuleShape(0.25, 0.25);
  	ghostObject = new btPairCachingGhostObject();
	ghostObject->setWorldTransform(btTransform(btQuaternion(0, 0, 0),origin));
	physics_manager->dynamicsWorld->getPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
	ghostObject->setCollisionShape(convexShape);
	ghostObject->setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT);
	controller = new btKinematicCharacterController(ghostObject,convexShape,0.5);
	physics_manager->dynamicsWorld->addCollisionObject(ghostObject,btBroadphaseProxy::KinematicFilter, btBroadphaseProxy::StaticFilter|btBroadphaseProxy::DefaultFilter);
	physics_manager->dynamicsWorld->addAction(controller);


  	convexShape = new btCapsuleShape(0.275, 0.275);


    physics_manager->AddRigidBody(convexShape, origin, mass, restitution, inertia, 
    							  kinematic, "Character", physicsIndex);


    m_mesh = new Mesh();
    m_mesh->LoadVertexData(objFilename);	
}

Character::~Character()
{

}

void Character::Render(Shader* shader)
{
	m_mesh->Render(shader);
}

void Character::ShadowRender(GLuint glint)
{
	m_mesh->ShadowRender(glint);
}