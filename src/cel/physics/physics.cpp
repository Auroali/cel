#include "physics.h"
#include <bullet/LinearMath/btVector3.h>
#include <bullet/LinearMath/btAlignedObjectArray.h>
#include <bullet/btBulletDynamicsCommon.h>

namespace cel::physics {
    std::unique_ptr<physics> create() {
        btCollisionConfiguration* conf = new btDefaultCollisionConfiguration();
        btCollisionDispatcher* dispatcher = new btCollisionDispatcher(conf);
        btBroadphaseInterface* broad = new btDbvtBroadphase();
        btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver();

        btDynamicsWorld* world = new btDiscreteDynamicsWorld(dispatcher, broad, solver, conf);
        world->setGravity(btVector3(0, -9.81f, 0));
        return std::make_unique<physics>(world);
    }
    physics::physics(btDynamicsWorld* world) {
        this->world = world;
    }
}