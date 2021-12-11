#pragma once
#include <bullet/LinearMath/btVector3.h>
#include <bullet/LinearMath/btAlignedObjectArray.h>
#include <bullet/btBulletDynamicsCommon.h>

#include "cel/framework/components/component.h"
#include "cel/reflect/reflect.h"

namespace cel {
    class rigidbody_component : public component {
        REFLECT();
        btRigidBody* rb;
    public:
        rigidbody_component();
        ~rigidbody_component();
        virtual void on_attach(std::shared_ptr<cel::object> obj) override;
        virtual void fixed_update() override;
    };
}