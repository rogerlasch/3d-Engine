

#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include"BodyHandler.h"
namespace gpp{

class octreenode;
class WorldProperties;
class RigidBody{
public:
uint32 id;
uint32 rbflags;

decimal mass;
decimal invMass;

decimal restitution;
decimal staticFriction;
decimal dynamicFriction;

vector3d velocity;
vector3d angularVelocity;
vector3d forces;
vector3d torque;

matrix3x3 inertia;
matrix3x3 invInertia;

GeometricShape* hbody;
void* userData;

public:
BodyHandler* handler;
octreenode* hnode;
public:
RigidBody();
RigidBody(const RigidBody& rb)=delete;
RigidBody& operator=(const RigidBody& rb)=delete;
virtual ~RigidBody();

virtual void cleanup();
virtual std::string toString()const;

    // Getters and Setters

inline void setUserData(void* userdata){this->userData=userdata;}
inline void* getUserData()const{return this->userData;}
inline void setBodyHandler(BodyHandler* handler){this->handler->copyFrom(handler);}
inline BodyHandler getBodyHandler()const{return *this->handler;}
inline     void setId(uint32 id) { this->id = id; }
inline     uint32 getId() const { return this->id; }

inline     void setMass(float mass) {
 this->mass = mass;
setInverseMass(((mass>0.0f) ? 1/mass : 0.0f));
//calculateInertia();
 }
inline     float getMass() const { return this->mass; }

inline     void setInverseMass(float inverseMass) { this->invMass= inverseMass; }
inline     float getInverseMass() const { return this->invMass; }

inline     void setRestitution(float restitution) { this->restitution = restitution; }
inline     float getRestitution() const { return this->restitution; }

inline void setStaticFriction(float sf){this->staticFriction=sf;}
inline float getStaticFriction()const{return this->staticFriction;}

inline void setDynamicFriction(float kf){this->dynamicFriction=kf;}
inline float getDynamicFriction()const{return this->dynamicFriction;}

//inline     void setLastPosition(const vector3d& lastPosition) { this->lastPosition = lastPosition; }
//inline     vector3d getLastPosition() const { return this->lastPosition; }

//inline     void setPosition(const vector3d& position) { this->position = position; }
inline     vector3d getPosition() const { return ((hbody==NULL) ? vector3d() : hbody->position);}

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

inline     void setOrientation(const quaternion& orientation) {if(hbody!=NULL) hbody->setOrientation(orientation);}
inline     quaternion getOrientation() const { return ((hbody==NULL) ? quaternion() : hbody->getOrientation());}

//virtual vector3d getVelocityAtPoint(const vector3d& point);
virtual inline void addForce(const vector3d& force){if(mass>0.0f) this->forces+=force;}
virtual inline void addTorque(const vector3d& tr){if(mass>0.0f) this->torque+=tr;}
virtual void applyLinearImpulse(const vector3d& impulse);
/*
virtual void applyAngularImpulse(const vector3d& impulse, const vector3d& point);
virtual void addTorque(const vector3d& tr, const vector3d& point);
*/

inline void setGeometricShape(GeometricShape* sh){if(hbody!=NULL) delete hbody; this->hbody=sh;}
inline GeometricShape* getGeometricShape()const{return this->hbody;}

inline void setOctreeNode(octreenode* hnode){this->hnode=hnode;}
inline octreenode* getOctreeNode()const{return this->hnode;}

virtual void translate(const vector3d& ts);
virtual void rotate(const quaternion& orientation);
virtual void rotate(const vector3d& origin, const quaternion& orientation);

virtual bool preStep(WorldProperties* props);
virtual void step(decimal dt);
virtual void onLoop();
virtual void beginContact(RigidBody* rb);
virtual void updateContact(RigidBody* rb);
virtual void endContact(RigidBody* rb);
friend class gpp_world;
};
}
#endif
