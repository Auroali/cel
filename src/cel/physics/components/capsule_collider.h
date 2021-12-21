#pragma once
#include "collider.h"

namespace cel {
    class capsule_collider_component : public collider_component {
        float size;
        float radius;
        REFLECT();
    public:
        capsule_collider_component() : size(1.0f), radius(0.5f) {}
        capsule_collider_component(float size) : size(size), radius(0.5f) {}
        capsule_collider_component(float size, float radius) : size(size), radius(radius) {}
        virtual void on_attach(std::shared_ptr<object> obj) override;
    };
}