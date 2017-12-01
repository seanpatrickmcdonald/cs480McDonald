
#include "physics.h"
#include "engine.h"



struct MyContactResultCallback : public btCollisionWorld::ContactResultCallback
{
	bool collision = false; 
	//int obj2;

    btScalar addSingleResult(btManifoldPoint& cp,
        const btCollisionObjectWrapper* colObj0Wrap,
        int partId0,
        int index0,
        const btCollisionObjectWrapper* colObj1Wrap,
        int partId1,
        int index1)
    {
      collision = true;
      return btScalar(0);
    }
};


bool callbackFunc(btManifoldPoint& cp, const btCollisionObjectWrapper* obj1, int id1, int index1, const btCollisionObjectWrapper* obj2, int id2, int index2)
{	
	return true;
}

PhysicsManager::PhysicsManager()
{
    // Initialize Bullet
    broadphase             = new btDbvtBroadphase();
    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher             = new btCollisionDispatcher(collisionConfiguration);
    solver                 = new btSequentialImpulseConstraintSolver();
    dynamicsWorld          = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

    // Add gravity
    dynamicsWorld->setGravity(btVector3(0, -10, 0));

    gContactProcessedCallback = (ContactProcessedCallback)callbackFunc;
}

PhysicsManager::~PhysicsManager()
{
    // Free Bullet
    for(signed int i = 0; i < dynamicsWorld->getNumCollisionObjects(); i++)
    {
        dynamicsWorld->removeRigidBody( btRigidBody::upcast(dynamicsWorld->getCollisionObjectArray()[i]) );
    }

    delete dynamicsWorld;
    delete broadphase;
    delete collisionConfiguration;
    delete dispatcher;
    delete solver;

    dynamicsWorld          = NULL;
    broadphase             = NULL;
    collisionConfiguration = NULL;
    dispatcher             = NULL;
    solver                 = NULL;
}

//restitution is the difference in velocity after a collision of an object new velocity/prev velocity
void PhysicsManager::AddRigidBody(btCollisionShape* collisionShape, btVector3 origin, btScalar mass, btScalar restitution, btVector3 inertia, bool kinematic, std::string objName, unsigned int user_index)
{ 
    btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), origin));
    if(mass > 0)
        collisionShape->calculateLocalInertia(mass, inertia);
    
    btRigidBody::btRigidBodyConstructionInfo constructionInfo(mass, motionState, collisionShape, inertia);
    btRigidBody* body = new btRigidBody(constructionInfo);
    body->setRestitution(restitution);
    body->setActivationState(DISABLE_DEACTIVATION);
    
    if(kinematic)
    {
        body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
    }

	body->setUserIndex(user_index);

    if (dynamicsWorld)
        dynamicsWorld->addRigidBody(body);
}

int PhysicsManager::GetNumObjects()
{
    return dynamicsWorld->getNumCollisionObjects();
}

glm::mat4 PhysicsManager::GetModelMatrixAtIndex(int index)
{
    if(index < 0 || index >= GetNumObjects())
    {
        cout << "ERROR! PhysicsManager::GetModelMatrixAtIndex tried to access: " << index << " which is not between 0 and " << GetNumObjects() - 1 << "." << endl;
        return glm::mat4(1.0f);
    }

    btTransform trans;
    btScalar m[16];
    btRigidBody* object = btRigidBody::upcast(dynamicsWorld->getCollisionObjectArray()[index]);
    object->getMotionState()->getWorldTransform(trans);
    trans.getOpenGLMatrix(m);
    glm::mat4 modelMatrix = glm::make_mat4(m);
    return modelMatrix;
}

void PhysicsManager::Update(unsigned int dt)
{	
    dynamicsWorld->stepSimulation(dt, 1); //1 is max time step
}

//applies force in direction
void PhysicsManager::ApplyForceAtIndex(btVector3 force, int index)
{
    if(index < 0 || index >= GetNumObjects())
    {
        cout << "ERROR! PhysicsManager::ApplyForceAtIndex tried to access: " << index << " which is not between 0 and " << GetNumObjects() << "." << endl;
        return;
    }

    btRigidBody* object = btRigidBody::upcast(dynamicsWorld->getCollisionObjectArray()[index]);
    object->applyCentralImpulse(force);
    object->clearForces();
}

//applies torque in direction
void PhysicsManager::ApplyTorqueAtIndex(btVector3 torque, int index)
{
    if(index < 0 || index >= GetNumObjects())
    {
        cout << "ERROR! PhysicsManager::ApplyTorqueAtIndex tried to access: " << index << " which is not between 0 and " << GetNumObjects() << "." << endl;
        return;
    }

    btRigidBody* object = btRigidBody::upcast(dynamicsWorld->getCollisionObjectArray()[index]);
    object->applyImpulse(torque, btVector3(-1.0, 0, 0));
    object->clearForces();
}


void PhysicsManager::MoveKinematic(btVector3 movementVector)
{
  btRigidBody* object = btRigidBody::upcast(dynamicsWorld->getCollisionObjectArray()[3]);

  if (object != nullptr)
  {
    btTransform tr;
    object->getMotionState()->getWorldTransform(tr);
    tr.setOrigin( tr.getOrigin() + movementVector);

    object->getMotionState()->setWorldTransform(tr);
  }

  else
    std::cout << "invalid object" << std::endl; 
}

glm::vec3 PhysicsManager::getTranslation(int index)
{
  btRigidBody* object = btRigidBody::upcast(dynamicsWorld->getCollisionObjectArray()[index]);
  btTransform tr;
  object->getMotionState()->getWorldTransform(tr);
  btVector3 btTranslation = tr.getOrigin();
  glm::vec3 translation = glm::vec3(btTranslation[0], btTranslation[1], btTranslation[2]);

  return translation;
}
