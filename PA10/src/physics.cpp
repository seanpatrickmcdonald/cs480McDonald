
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
    //dynamicsWorld->setGravity(btVector3(0, 0, 0.5));

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
    body->setLinearFactor(btVector3(1, 0, 1));
    

    if(kinematic)
    {
        body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
    }

    if (objName == "RightFlipper")
    {
    	btHingeConstraint *rightFlipperHinge = new btHingeConstraint(*body, btVector3(0.0008715, 1.01382, 0.0001), btVector3(0, 1, 0));
    	rightFlipperHinge->setLimit(-1.5, 0, 1.0, 0.3, 0.0);
    	dynamicsWorld->addConstraint(rightFlipperHinge);
	}

	if (objName == "LeftFlipper" && false)
    {
    	btHingeConstraint *leftFlipperHinge = new btHingeConstraint(*body, btVector3(0.0008715, 1.01382, 0.0001), btVector3(0, 1, 0));
    	leftFlipperHinge->setLimit(-1.5, 0, 1.0, 0.3, 0.0);
    	dynamicsWorld->addConstraint(leftFlipperHinge);
    }

	if (objName == "  ")
	{
		//body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
	}

	body->setUserIndex(user_index);

    if (dynamicsWorld)
        dynamicsWorld->addRigidBody(body);

	if (objName == "Ball")
	{
		body->setGravity(btVector3(0, -10, 0.65));
		ball = body;

		btTransform frameInA = btTransform::getIdentity();
		frameInA.setOrigin(btVector3(0., 0., 0.));
		btGeneric6DofConstraint *constraint = new btGeneric6DofConstraint(*body, frameInA, false);
		constraint->setLinearLowerLimit(btVector3(-10, -10, -1.5));
		constraint->setLinearUpperLimit(btVector3(9, 1.5, 9));

		dynamicsWorld->addConstraint(constraint);

	}

	if (objName == "50Target")
	{
		fiftyTarget.push_back(body);
	}

	if (objName == "10Target")
	{
		tenTarget.push_back(body);
	}

	if (objName == "50Trigger")
	{		
		fiftyTarget.push_back(body);
		body->setCollisionFlags(body->getCollisionFlags() |
    btCollisionObject::CF_NO_CONTACT_RESPONSE);
	}

	if (objName == "25Trigger")
	{		
		twentyfiveTarget.push_back(body);
		body->setCollisionFlags(body->getCollisionFlags() |
    btCollisionObject::CF_NO_CONTACT_RESPONSE);
	}

	if (objName == "10Trigger")
	{		
		tenTarget.push_back(body);
		body->setCollisionFlags(body->getCollisionFlags() |
    btCollisionObject::CF_NO_CONTACT_RESPONSE);
	}
}

int PhysicsManager::GetNumObjects()
{
    return dynamicsWorld->getNumCollisionObjects();
}

glm::mat4 PhysicsManager::GetModelMatrixAtIndex(int index)
{
    if(index < 0 || index >= GetNumObjects())
    {
        cout << "ERROR! PhysicsManager::GetModelMatrixAtIndex tried to access: " << index << " which is not between 0 and " << GetNumObjects() << "." << endl;
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
    dynamicsWorld->stepSimulation(1); //1 is max time step

    MyContactResultCallback targetHitTest;

    for (unsigned int i = 0; i < fiftyTarget.size(); i++)
    {
	    dynamicsWorld->contactPairTest(ball, fiftyTarget[i], targetHitTest);
	   
	    if (targetHitTest.collision)
        {
	    	Engine::score += 50;
            break;
        }
	}

    if (!targetHitTest.collision)
    {
        for (unsigned int i = 0; i < twentyfiveTarget.size(); i++)
        {
            dynamicsWorld->contactPairTest(ball, twentyfiveTarget[i], targetHitTest);
           
            if (targetHitTest.collision)
                Engine::score += 25;
        }  
    }

    if (!targetHitTest.collision)
    {
        for (unsigned int i = 0; i < tenTarget.size(); i++)
        {
    	    dynamicsWorld->contactPairTest(ball, tenTarget[i], targetHitTest);
    	   
    	    if (targetHitTest.collision)
    	    	Engine::score += 10;
    	}
    }

	//double asdf;

	//if (dt > 0)
	//asdf = 1000/ dt;

	//std::cout << "FPS: " << asdf << std::endl;

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

void PhysicsManager::RotateFlipper(int index, float angle)
{
  btRigidBody* object = btRigidBody::upcast(dynamicsWorld->getCollisionObjectArray()[index]);

  if (object != nullptr)
  {
    btTransform tr;
    object->getMotionState()->getWorldTransform(tr);

    btQuaternion rotation(btVector3(0, 1, 0), angle);

    tr.setRotation(rotation);

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

void PhysicsManager::ResetBall()
{
  btRigidBody* object = btRigidBody::upcast(dynamicsWorld->getCollisionObjectArray()[0]);
  object->setCenterOfMassTransform(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0.34046, 0.9980, 0.69679)));
  object->clearForces();
  object->setAngularVelocity(btVector3(0, 0, 0));
  object->setLinearVelocity(btVector3(0, 0, 0));
}
