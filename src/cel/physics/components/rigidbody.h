#pragma once
#include <bullet/LinearMath/btVector3.h>
#include <bullet/LinearMath/btAlignedObjectArray.h>
#include <bullet/btBulletDynamicsCommon.h>

#include "cel/framework/components/component.h"
#include "collider.h"
#include "cel/reflect/reflect.h"

namespace cel {
    /**
     * Component that enables an object to have physics
     * <br> Requires the parent object to have a collider component of some kind
     * <br> The collider component can be added after the rigidbody, as long as it is before the parent object is attached to a scene.
     * <br> If the parent object is already attached to a scene, it MUST already have a collider component, or things will not work properly!
     * <br> A collider can be set via rigidbody_component::set_collider
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
        
        /**
         * @brief In the event that either the wrong collider was used,
         * or that a collider was not detected, it can be manually set here.
         * 
         * @param collider the collider component to use
         */
        void set_collider(std::weak_ptr<cel::collider_component> collider);
    };
}