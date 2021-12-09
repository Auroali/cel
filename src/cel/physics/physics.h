#pragma once
#include <memory>
#include <bullet/LinearMath/btVector3.h>
#include <bullet/LinearMath/btAlignedObjectArray.h>
#include <bullet/btBulletDynamicsCommon.h>

class btDynamicsWorld;

namespace cel::physics {
    class physics {
        btDynamicsWorld* world;
    public:
        /**
         * Initializes a physics instance
         */
        static std::unique_ptr<physics> create();
        physics(btDynamicsWorld* world);
        void step();
    };
}