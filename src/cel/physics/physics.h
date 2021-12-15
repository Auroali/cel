#pragma once
#include <memory>
#include <bullet/LinearMath/btVector3.h>
#include <bullet/LinearMath/btAlignedObjectArray.h>
#include <bullet/btBulletDynamicsCommon.h>
#include "components/rigidbody.h"

class btDynamicsWorld;

/**
 * @namespace cel::physics Contains the cel physics interface
 */
namespace cel::physics {
    /**
     * An interface for the physics engine
     */
    class physics {
        btDynamicsWorld* world;
        friend class cel::rigidbody_component;
    public:
        /**
         * Initializes a physics instance
         */
        static std::unique_ptr<physics> create();
        physics(btDynamicsWorld* world);
        /**
         * @brief Steps the physics simulation
         * 
         * @note Do not call this! It is already called by cel.
         */
        void step();
    };
}