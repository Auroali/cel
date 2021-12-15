#include "physics.h"
#include <cel/time.h>
#include "components/rigidbody.h"
#include "cel/framework/scene.h"
#include "components/collider.h"
#include "components/cube_collider.h"
#include "components/sphere_collider.h"
#include "cel/logger.h"

namespace cel::physics {
    std::unique_ptr<physics> physics::create() {
        btCollisionConfiguration* conf = new btDefaultCollisionConfiguration();
        btCollisionDispatcher* dispatcher = new btCollisionDispatcher(conf);
        btBroadphaseInterface* broad = new btDbvtBroadphase();
        btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver();

        btDynamicsWorld* world = new btDiscreteDynamicsWorld(dispatcher, broad, solver, conf);
        world->setGravity(btVector3(0, 9.81f, 0));
        return std::make_unique<physics>(world);
    }
    physics::physics(btDynamicsWorld* world) {
        this->world = world;
    }
    void physics::step() {
        world->stepSimulation(cel::time::fixed_delta_time);
    }

    
    
}

REFLECT_COMPONENT_DEFINE(cel::rigidbody_component)
REFLECT_MEMBER_WITH(rb, 0)
REFLECT_MEMBER(friction)
REFLECT_MEMBER(mass)
REFLECT_MEMBER(is_kinematic)
REFLECT_END()

void cel::rigidbody_component::fixed_update() {
    if(auto obj = parent.lock()) {
        if(rb) {
            btQuaternion quat = rb->getWorldTransform().getRotation();
            btVector3 pos = rb->getWorldTransform().getOrigin();
            obj->trans.rot = glm::quat(quat.x(), quat.y(), quat.z(), quat.w());
            obj->trans.pos = glm::vec3(pos.x(), pos.y(), pos.z());
        }
    }
}
void cel::rigidbody_component::on_scene_added(std::shared_ptr<scene> scene) {
    //btRigidBody::btRigidBodyConstructionInfo info = btRigidBody::btRigidBodyConstructionInfo(1.0, );
    if(auto obj = parent.lock()) {
        btVector3 position = btVector3(obj->trans.pos.x,obj->trans.pos.y,obj->trans.pos.z);
        btQuaternion rotation = btQuaternion(obj->trans.rot.x,obj->trans.rot.y,obj->trans.rot.z,obj->trans.rot.w);
        btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(rotation, position));
        btVector3 bodyInertia;
        btCollisionShape* shape = nullptr;
        auto components =  obj->get_components();
        auto it = std::find_if(components.begin(), components.end(), [&](std::weak_ptr<cel::component> comp) {return comp.lock() && std::dynamic_pointer_cast<cel::collider_component>(comp.lock());});
        if (it != components.end()) {
            if(auto locked_c = (*it).lock()) {
                shape = std::dynamic_pointer_cast<cel::collider_component>(locked_c)->shape;
            }
        }
        if(mass != 0 && shape != nullptr) {
            shape->calculateLocalInertia(mass, bodyInertia);
        }
        btRigidBody::btRigidBodyConstructionInfo bodyCI = btRigidBody::btRigidBodyConstructionInfo(mass,motionState,shape,bodyInertia);
        bodyCI.m_restitution = 0.2f;
        bodyCI.m_friction = friction;
        this->rb = new btRigidBody(bodyCI);
        if(rb->getCollisionShape())
            rb->getCollisionShape()->setLocalScaling(btVector3(obj->trans.scale.x,obj->trans.scale.y,obj->trans.scale.z));
        // Enable the kinematic flag, if true
        set_kinematic(is_kinematic);
        scene->get_phys()->world->addRigidBody(this->rb);
    }
}
cel::rigidbody_component::~rigidbody_component() {
    if(rb) {
        if(auto scene = cel::scene::get_active_scene().lock()) {
            if(rb->isInWorld())
                scene->get_phys()->world->removeRigidBody(rb);
        }
        if(rb->getMotionState())
            delete rb->getMotionState();
        delete rb;
    }
}

void cel::rigidbody_component::set_kinematic(bool kinematic) {
    this->is_kinematic = kinematic;
    if(rb) {
        if(kinematic)
            rb->setCollisionFlags(rb->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
        else
            rb->setCollisionFlags(rb->getCollisionFlags() & ~btCollisionObject::CF_KINEMATIC_OBJECT);
    }
        
}
void cel::rigidbody_component::set_friction(float friction) {
    this->friction = friction;
    if(rb)
        rb->setFriction(friction);
}
void cel::rigidbody_component::set_mass(float mass) {
    this->mass = mass;
    if(rb) {
        btVector3 intertia;
        if(rb->getCollisionShape())
            rb->getCollisionShape()->calculateLocalInertia(mass, intertia);
        rb->setMassProps(mass, intertia);
    }
}
/**
 * 
 * Collider Definitions
 * 
 */
namespace cel {
    // Empty Collision
    REFLECT_COMPONENT_DEFINE(cel::collider_component)
    REFLECT_MEMBER_WITH(shape, 0)
    REFLECT_END()
    // Sphere Collision
    REFLECT_COMPONENT_DEFINE(cel::sphere_collider_component)
    REFLECT_MEMBER(radius)
    REFLECT_MEMBER_WITH(shape, 0)
    REFLECT_END()
    // Cube Collision
    REFLECT_COMPONENT_DEFINE(cel::cube_collider_component)
    REFLECT_MEMBER(size)
    REFLECT_MEMBER_WITH(shape, 0)
    REFLECT_END()

    collider_component::~collider_component() {
        if(shape)
            delete shape;
    }
    void collider_component::on_attach(std::shared_ptr<object> obj) {
        // Warn the user that this is in fact not the right component, as it doesn't provide collision.
        LOG_WARN(fmt::format("A component of type cel::collider_component was added to object '{}' at address {}. This is likely not the component you want to use!", obj->name, (size_t)obj.get()));
    }
    // Initialize the shape in on_attach to allow things like the scene loader to set the properties of the shape
    void cube_collider_component::on_attach(std::shared_ptr<object> obj) {
        shape = new btBoxShape(btVector3(size.x, size.y, size.z));
    }
    void sphere_collider_component::on_attach(std::shared_ptr<object> obj) {
        shape = new btSphereShape(radius);
    }
}
