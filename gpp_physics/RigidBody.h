
#ifndef RIGIDBODY_H
#define RIGIDBODY_H

namespace gpp{

enum RIGIDBODY_FLAGS{
RB_SLEEP=(1<<0)
};

enum RIGIDBODY_GEOMETRY_TYPES{
GTYPE_SPHERE=(1<<0),
GTYPE_BOX=(1<<1),
GTYPE_CAPSULE=(1<<2),
GTYPE_CYLINDER=(1<<3)
};

class CollisionInfo;
class RigidBody{
public:
uint32 id;
uint32 rbflags;
uint32 geometricType;
float mass;
float inverseMass;
float restitution;
float staticFriction;
float dynamicFriction;
vector3d lastPosition;
vector3d position;

vector3d velocity;
vector3d angularVelocity;

vector3d forces;
vector3d torque;

vector3d vecInertia;
matrix3x3 inertia;
matrix3x3 invInertia;

quaternion orientation;

RigidBody* hsurface;
void* userdata;

RigidBody();
RigidBody(const RigidBody& rb)=delete;
RigidBody& operator=(const RigidBody& rb)=delete;
virtual ~RigidBody();

virtual void cleanup();
virtual std::string toString()const;

    // Getters and Setters

inline void setUserData(void* userdata){this->userdata=userdata;}
inline void* getUserData()const{return this->userdata;}
inline     void setId(uint32 id) { this->id = id; }
inline     uint32 getId() const { return this->id; }

inline void setGeometricType(uint32 type){this->geometricType=type;}
inline uint32 getGeometricType()const{return this->geometricType;}

inline     void setMass(float mass) {
 this->mass = mass;
setInverseMass(((mass>0.0f) ? 1/mass : 0.0f));
calculateInertia();
 }
inline     float getMass() const { return this->mass; }

inline     void setInverseMass(float inverseMass) { this->inverseMass = inverseMass; }
inline     float getInverseMass() const { return this->inverseMass; }

inline     void setRestitution(float restitution) { this->restitution = restitution; }
inline     float getRestitution() const { return this->restitution; }

inline void setStaticFriction(float sf){this->staticFriction=sf;}
inline float getStaticFriction()const{return this->staticFriction;}

inline void setDynamicFriction(float kf){this->dynamicFriction=kf;}
inline float getDynamicFriction()const{return this->dynamicFriction;}

inline     void setLastPosition(const vector3d& lastPosition) { this->lastPosition = lastPosition; }
inline     vector3d getLastPosition() const { return this->lastPosition; }

inline     void setPosition(const vector3d& position) { this->position = position; }
inline     vector3d getPosition() const { return this->position; }

inline     void setVelocity(const vector3d& velocity) { this->velocity = velocity; }
inline     vector3d getVelocity() const { return this->velocity; }

inline     void setAngularVelocity(const vector3d& angularVelocity) { this->angularVelocity = angularVelocity; }
inline     vector3d getAngularVelocity() const { return this->angularVelocity; }

inline     void setForces(const vector3d& forces) { this->forces = forces; }
inline     vector3d getForces() const { return this->forces; }

inline     void setTorque(const vector3d& torque) { this->torque = torque; }
inline     vector3d getTorque() const { return this->torque; }

inline     void setInertia(const matrix3x3& inertia) { this->inertia = inertia; }
inline     matrix3x3 getInertia() const { return this->inertia; }

inline     void setInvInertia(const matrix3x3& invInertia) { this->invInertia = invInertia; }
inline     matrix3x3 getInvInertia() const { return this->invInertia; }

inline     void setOrientation(const quaternion& orientation) { this->orientation = orientation; }
inline     quaternion getOrientation() const { return this->orientation; }

inline void setSurface(RigidBody* hsurface){this->hsurface=hsurface;}
inline RigidBody* getSurface()const{return this->hsurface;}

virtual vector3d getVelocityAtPoint(const vector3d& point);
virtual inline void addForce(const vector3d& force){if(mass>0.0f) this->forces+=force;}
virtual inline void addTorque(const vector3d& tr){if(mass>0.0f) this->torque+=tr;}
virtual void applyLinearImpulse(const vector3d& impulse);
virtual void applyAngularImpulse(const vector3d& impulse, const vector3d& point);

virtual void addTorque(const vector3d& tr, const vector3d& point);
virtual void updateInertiaTensor();
virtual void calculateInertia();

virtual void prepareBody(float dt, const vector3d& gravity);
virtual void beginContact(RigidBody* rb, CollisionInfo* info);
virtual void endContact(RigidBody* rb, CollisionInfo* info);
virtual float getAabb(vector3d& min, vector3d& max)const;
};
}

#endif
