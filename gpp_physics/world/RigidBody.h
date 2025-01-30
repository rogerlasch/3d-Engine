

#ifndef RIGIDBODY_H
#define RIGIDBODY_H

namespace gpp {

class octreenode;
class WorldInfo;
class CollisionInfo;

class RigidBody {
public:
    uint32 id;
    uint32 rbflags;

    decimal mass;
decimal invMass;
    decimal restitution;
    decimal staticFriction;
    decimal dynamicFriction;
decimal linearDamping;
decimal angularDamping;

    vector3d linearVelocity;
    vector3d angularVelocity;
    vector3d forces;
    vector3d torque;

    matrix3x3 inertia;
    matrix3x3 invInertia;

    GeometricShape* hbody;
    void* userData;
    octreenode* hnode;

    RigidBody();
    virtual ~RigidBody();

    // Remover cópia
    RigidBody(const RigidBody& rb) = delete;
    RigidBody& operator=(const RigidBody& rb) = delete;

    // Getters e Setters essenciais
    void setUserData(void* userdata) { this->userData = userdata; }
    void* getUserData() const { return this->userData; }

    void setId(uint32 id) { this->id = id; }
    uint32 getId() const { return this->id; }

    void setMass(decimal mass) {
this->mass = mass;
if(mass>0.0f){
setInvMass(1/mass);
}
}

    decimal getMass() const { return this->mass; }

void setInvMass(decimal invMass){this->invMass=invMass;}
decimal getInvMass()const{return this->invMass;}

    void setRestitution(decimal restitution) { this->restitution = restitution; }
    decimal getRestitution() const { return this->restitution; }

    void setStaticFriction(decimal sf) { this->staticFriction = sf; }
    decimal getStaticFriction() const { return this->staticFriction; }

    void setDynamicFriction(decimal df) { this->dynamicFriction = df; }
    decimal getDynamicFriction() const { return this->dynamicFriction; }

    void setLinearDamping(decimal df) { this->linearDamping = df; }
    decimal getLinearDamping() const { return this->linearDamping; }

    void setAngularDamping(decimal df) { this->angularDamping = df; }
    decimal getAngularDamping() const { return this->angularDamping; }

    vector3d getPosition() const { return (hbody ? hbody->position : vector3d()); }

    void setLinearVelocity(const vector3d& vel) { this->linearVelocity = vel; }
    vector3d getLinearVelocity() const { return this->linearVelocity; }

    void setAngularVelocity(const vector3d& angVel) { this->angularVelocity = angVel; }
    vector3d getAngularVelocity() const { return this->angularVelocity; }

    void setForces(const vector3d& f) { this->forces = f; }
    vector3d getForces() const { return this->forces; }

    void setTorque(const vector3d& t) { this->torque = t; }
    vector3d getTorque() const { return this->torque; }

    void setInertia(const matrix3x3& inertia) { this->inertia = inertia; }
    matrix3x3 getInertia() const { return this->inertia; }

    void setInvInertia(const matrix3x3& invInertia) { this->invInertia = invInertia; }
    matrix3x3 getInvInertia() const { return this->invInertia; }

    void setOrientation(const quaternion& orientation) {
        if (hbody) hbody->setOrientation(orientation);
    }
    quaternion getOrientation() const {
        return (hbody ? hbody->getOrientation() : quaternion());
    }

    // GeometricShape
    void setGeometricShape(GeometricShape* shape) {
        if (hbody) delete hbody;
        this->hbody = shape;
    }
    GeometricShape* getGeometricShape() const { return this->hbody; }

    // Octree node
    void setOctreeNode(octreenode* node) { this->hnode = node; }
    octreenode* getOctreeNode() const { return this->hnode; }

    // Métodos para aplicar forças
    void addForce(const vector3d& force) {
        if (mass > 0.0f) this->forces += force;
    }

    void addTorque(const vector3d& torque) {
        if (mass > 0.0f) this->torque += torque;
    }

    // Transformações e simulação
    virtual void translate(const vector3d& translation);
    virtual void rotate(const quaternion& orientation);
    virtual void rotate(const vector3d& origin, const quaternion& orientation);

    virtual bool preStep(WorldInfo* props);
    virtual void step(decimal dt);
virtual void solveCollision(RigidBody* r2, CollisionInfo* info);
    virtual void onLoop();

    // Métodos de contato (virtuals para polimorfismo)
    virtual void beginContact(RigidBody* other);
    virtual void updateContact(RigidBody* other);
    virtual void endContact(RigidBody* other);

    virtual void cleanup();
    virtual std::string toString() const;

static vector3d computeFriction(const vector3d& normal, const vector3d& relativeVel, decimal staticFriction, decimal dynamicFriction);
};

} // namespace gpp

#endif // RIGIDBODY_H
