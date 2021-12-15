#pragma once
#include <bullet/LinearMath/btVector3.h>
#include <bullet/LinearMath/btAlignedObjectArray.h>
#include <bullet/btBulletDynamicsCommon.h>

#include "cel/framework/components/component.h"
#include "cel/reflect/reflect.h"

namespace cel {
    /**
     * Component that enables an object to have physics
     */
    class rigidbody_component : public component {
        REFLECT();
        btRigidBody* rb;
        bool is_kinematic;
        float friction;
        float mass;
    public:
        /**
         * @brief Constructs a new rigidbody component
         */
        rigidbody_component() : friction(1.0f), mass(1.0f) { cel::component(); }
        /**
         * @brief Constructs a rigidbody component with custom mass
         * 
         * @param mass the mass of the rigidbody
         */
        rigidbody_component(float mass) : friction(1.0f), mass(mass) { cel::component(); }
        /**
         * @brief Constructs a rigidbody component with custom mass and friction
         * 
         * @param mass the mass of the rigidbody
         * @param friction the friction of the rigidbody
         */
        rigidbody_component(float mass, float friction) : friction(friction), mass(mass) { cel::component(); }
        ~rigidbody_component();

        virtual void on_scene_added(std::shared_ptr<scene> scene) override;
        virtual void fixed_update() override;

        /**
         * @brief Sets whether or not this rigidbody is kinematic
         * 
         * @param kinematic whether to enable or disable kinematic for this component
         */
        void set_kinematic(bool kinematic);
        /**
         * @brief Sets the friction of this rigidbody
         * 
         * @param friction the amount of friction for this rigidbody
         */
        void set_friction(float friction);
        /**
         * @brief Sets the mass of this rigidbody
         * 
         * @param mass the mass of this rigidbody
         */
        void set_mass(float mass);
    };
}