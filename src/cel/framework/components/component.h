#pragma once
#include "../object.h"
#include <memory>
#include "cel/reflect/reflect.h"
#include <iostream>

/**
 * Version of REFLECT_DEFINE that sets up
 * component-specific values
 */
#define REFLECT_COMPONENT_DEFINE(Typename) \
REFLECT_DEFINE(Typename) \
REFLECT_MEMBER_WITH(name, 0) \
REFLECT_MEMBER_WITH(trans, SERIALIZE)

namespace cel {
    /**
     * Base class for components that can be attached to objects
     */
    class component {
    private:
        // The owning object of this component
        std::weak_ptr<object> parent;
        friend class object;
    public:
        // Only exists for using reflect::solver::get_from() on a component pointer
        virtual cel::reflection::type* get_type() { return nullptr; }
        // The component's local transform
        transform trans;
        // The component's name
        std::string name;
        component();
        virtual ~component() {}
    };
}
