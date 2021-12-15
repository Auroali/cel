#pragma once
#include "cel/framework/components/component.h"
#include "cel/reflect/reflect.h"

namespace cel {
    class collider_component : public component {
    private:
        REFLECT();
    protected:
        btCollisionShape* shape;
        friend class rigidbody_component;
    public:
        ~collider_component();
        virtual void on_attach(std::shared_ptr<object> obj) override;
    };
}