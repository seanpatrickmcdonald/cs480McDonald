#include "character.h"


btVector3 Character::character_origin(0.0, 0.5, -15.0);

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
    m_physics_manager = physics_manager;
    //std::cout << "Character PhysicsIndex: " << physicsIndex << std::endl;

    //btVector3 origin     = btVector3(0.0, 0.5, -15.0);
    btScalar mass        = btScalar(0);
    btScalar restitution = 0.1;
    btVector3 inertia    = btVector3(0.0f, 0.0f, 0.0f);
    bool kinematic = true;


  	//convexShape = new btCapsuleShape(0.25, 0.25);
  	convexShape = new btCapsuleShape(0.25, 0.25);
  	ghostObject = new btPairCachingGhostObject();
	ghostObject->setWorldTransform(btTransform(btQuaternion(0, 0, 0),character_origin));
	physics_manager->dynamicsWorld->getPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
	ghostObject->setCollisionShape(convexShape);
	ghostObject->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);
	controller = new btKinematicCharacterController(ghostObject,convexShape,0.5);
	physics_manager->dynamicsWorld->addCollisionObject(ghostObject,btBroadphaseProxy::KinematicFilter, 
		           btBroadphaseProxy::StaticFilter|btBroadphaseProxy::DefaultFilter);
	physics_manager->dynamicsWorld->addAction(controller);

    physics_manager->player = ghostObject;
    physics_manager->controller = controller;

  	btCollisionShape *kineShape = new btCapsuleShape(0.3, 0.3);


    m_kinematic_body = physics_manager->AddRigidBody(kineShape, character_origin, mass, restitution, inertia, 
    							  kinematic, "Character", physicsIndex);


    m_mesh = new Mesh();
    m_mesh->LoadVertexData(objFilename);	
}

Character::~Character()
{
    if (m_mesh)
    {
        delete m_mesh;
        m_mesh = nullptr;
    }

}

void Character::Render(Shader* shader)
{
	m_mesh->Render(shader);
}

void Character::ShadowRender(GLuint glint)
{
	m_mesh->ShadowRender();
}

void Character::UpdateDynamicCollisions()
{
    /*

	btManifoldArray manifoldArray;
btBroadphasePairArray& pairArray =
    ghostObject->getOverlappingPairCache()->getOverlappingPairArray();
int numPairs = pairArray.size();

for (int i = 0; i < numPairs; ++i)
{
    manifoldArray.clear();

    const btBroadphasePair& pair = pairArray[i];

    btBroadphasePair* collisionPair =
        m_physics_manager->dynamicsWorld->getPairCache()->findPair(
            pair.m_pProxy0,pair.m_pProxy1);

    if (!collisionPair) continue;

    if (collisionPair->m_algorithm)
        collisionPair->m_algorithm->getAllContactManifolds(manifoldArray);

    for (int j = 0; j < manifoldArray.size(); j++)
    {
        btPersistentManifold* manifold = manifoldArray[j];

        bool isFirstBody = manifold->getBody0() == ghostObject;

        btScalar direction = isFirstBody ? btScalar(-1.0) : btScalar(1.0);

        for (int p = 0; p < manifold->getNumContacts(); ++p)
        {
            const btManifoldPoint& pt = manifold->getContactPoint(p);

            if (pt.getDistance() < 0.f)
            {
                const btVector3& ptA = pt.getPositionWorldOnA();
                const btVector3& ptB = pt.getPositionWorldOnB();
                const btVector3& normalOnB = pt.m_normalWorldOnB;

                // handle collisions here
            }
        }
    }
}
*/
}