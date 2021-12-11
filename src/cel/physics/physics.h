#pragma once
#include <memory>
#include <bullet/LinearMath/btVector3.h>
#include <bullet/LinearMath/btAlignedObjectArray.h>
#include <bullet/btBulletDynamicsCommon.h>
#include "components/rigidbody.h"

class btDynamicsWorld;

namespace cel::physics {
    class physics {
        btDynamicsWorld* world;
        friend class cel::rigidbody_component;
    public:
        /**
         * Initializes a physics instance
         */
        static std::unique_ptr<physics> create();
        physics(btDynamicsWorld* world);
        void step();
    };
}