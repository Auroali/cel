#pragma once
#include <memory>

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