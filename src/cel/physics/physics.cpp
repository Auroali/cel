#include "physics.h"
#include <cel/time.h>
#include "components/rigidbody.h"
#include "cel/framework/scene.h"

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
void cel::rigidbody_component::on_attach(std::shared_ptr<cel::object> obj) {
    //btRigidBody::btRigidBodyConstructionInfo info = btRigidBody::btRigidBodyConstructionInfo(1.0, );
    btVector3 position = btVector3(obj->trans.pos.x,obj->trans.pos.y,obj->trans.pos.z);
    btQuaternion rotation = btQuaternion(obj->trans.rot.x,obj->trans.rot.y,obj->trans.rot.z,obj->trans.rot.w);
    btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(rotation, position));
    btScalar bodyMass = 1.0;
    btVector3 bodyInertia;
    btCollisionShape* shape = new btSphereShape(1.0f);
    if(bodyMass != 0) {
        shape->calculateLocalInertia(bodyMass, bodyInertia);
    }
    btRigidBody::btRigidBodyConstructionInfo bodyCI = btRigidBody::btRigidBodyConstructionInfo(bodyMass,motionState,shape,bodyInertia);
    bodyCI.m_restitution = 0.2f;
    bodyCI.m_friction = 1.f;
    this->rb = new btRigidBody(bodyCI);
    rb->getCollisionShape()->setLocalScaling(btVector3(obj->trans.scale.x,obj->trans.scale.y,obj->trans.scale.z));
    if(auto scene = cel::scene::get_active_scene().lock()) {
        scene->get_phys()->world->addRigidBody(this->rb);
    }
    
}
cel::rigidbody_component::~rigidbody_component() {
    if(auto scene = cel::scene::get_active_scene().lock()) {
        if(rb) {
            if(rb->isInWorld())
                scene->get_phys()->world->removeRigidBody(rb);
            delete rb;
        }
    }
}
cel::rigidbody_component::rigidbody_component() {
    cel::component();
}