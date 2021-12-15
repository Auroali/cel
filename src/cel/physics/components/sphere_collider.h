#pragma once
#include "collider.h"

namespace cel {
    class sphere_collider_component : public collider_component {
    private:
        float radius;
        REFLECT();
    public:
        sphere_collider_component() : sphere_collider_component(1.0f) {}
        sphere_collider_component(float radius) : radius(radius) {}
        virtual void on_attach(std::shared_ptr<object> obj) override;
    };
}