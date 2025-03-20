

#ifndef RIGIDBODY_H
#define RIGIDBODY_H

namespace gpp {

class RigidBody {
private:

uint32 id;
void* userData;

    // Propriedades físicas
    decimal mass;
    decimal inverseMass;
    matrix3x3 inertia;
    matrix3x3 inverseInertia;
    vector3d linearVelocity;
    vector3d angularVelocity;
    vector3d force;
    vector3d torque;

    // Propriedades de estado
    GeometricShape* hbody;

    // Propriedades de colisão
    decimal restitution;
    decimal friction;

    // Flags e controles
    bool isStatic;
    bool useGravity;
    decimal linearDamping;
    decimal angularDamping;

RigidBody* hparent;
std::unordered_set<RigidBody*> childs;

public:
    // Construtor e destrutor
    RigidBody(RigidBody* hparent, decimal mass = 1.0f, GeometricShape* shape = nullptr);
    RigidBody(const RigidBody&) = delete;
    RigidBody& operator=(const RigidBody&) = delete;
    ~RigidBody();

std::string toString()const;

void setId(uint32 id){this->id=id;}
uint32 getId()const{return this->id;}

void setUserData(void* userData){this->userData=userData;}
void* getUserData()const{return this->userData;}

    // Aplicação de forças e torques
    void applyForce(const vector3d& force);
    void applyForceAtPoint(const vector3d& force, const vector3d& point);
    void applyTorque(const vector3d& torque);
    void clearForces();

    // Getters e Setters
    decimal getMass() const { return mass; }
    void setMass(decimal m);

decimal getFriction()const{return this->friction;}
void setFriction(decimal friction){this->friction=friction;}

    const vector3d& getLinearVelocity() const { return linearVelocity; }
    void setLinearVelocity(const vector3d& velocity) { linearVelocity = velocity; }

    const vector3d& getAngularVelocity() const { return angularVelocity; }
    void setAngularVelocity(const vector3d& velocity) { angularVelocity = velocity; }

Transform* getTransform() const { return hbody->getTransform();}
    void setTransform(const Transform& t){hbody->setTransform(t);}

    GeometricShape* getShape() const { return this->hbody;}
    void setShape(GeometricShape* s);

    bool getIsStatic() const { return isStatic; }
    void setIsStatic(bool staticFlag) { isStatic = staticFlag; }

    bool getUseGravity() const { return useGravity; }
    void setUseGravity(bool gravityFlag) { useGravity = gravityFlag; }

    // Utilidades
    matrix4x4 getWorldTransform() const;
    vector3d getVelocityAtPoint(const vector3d& point) const;

    // Colisão
    void update(decimal deltaTime);
    void resolveCollision(RigidBody* other, CollisionInfo* info);

void rotate(decimal x, decimal y, decimal z);
void rotate(const quaternion& q);
void translate(const vector3d& translation);

RigidBody* getParent()const{return this->hparent;}
void setParent(RigidBody* rb){this->hparent=rb;}

void pushBody(RigidBody* rb);
void removeBody(RigidBody* rb);
void removeAllBodies();

private:

    // Métodos internos
    void updateInertia();
};
} // namespace gpp
#endif // RIGIDBODY_H
