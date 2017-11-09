#ifndef __PHYSICS__H
#define __PHYSICS__H

#include "bullet/btBulletDynamicsCommon.h"
#include "json.hpp"
#include "graphics_headers.h"

enum collisionTypes
{
    COLLIDE_NOTHING = 0,
    COLLIDE_GROUND  = 1,
    COLLIDE_OBJECT  = 2
};

class PhysicsManager
{
    public:
        PhysicsManager();
        ~PhysicsManager();

        void AddRigidBody(btCollisionShape* collisionShape,
                          btVector3 origin     = btVector3(0,0,0),
                          btScalar mass        = btScalar(0),
                          btScalar restitution = btScalar(1.0f),
                          btVector3 inertia    = btVector3(0.0f, 0.0f, 0.0f),
                          bool kinematic       = false);

        glm::mat4 GetModelMatrixAtIndex(int index);
        int GetNumObjects();

        void Update(unsigned int dt);

        void ApplyForceAtIndex(btVector3 force, int index);

        void MoveKinematic(btVector3);
        glm::vec3 getTranslation(int index);

    private:
		
        btBroadphaseInterface* broadphase;
        btDefaultCollisionConfiguration* collisionConfiguration;
        btCollisionDispatcher *dispatcher;
        btSequentialImpulseConstraintSolver* solver;
        btDiscreteDynamicsWorld* dynamicsWorld;

};

#endif































