
#include<sstream>
#include<bitset>
#include"gpp_physics.h"
#include"RigidBody.h"

using namespace std;

namespace gpp
{
RigidBody::RigidBody(uint32 btype):
AABB()
{
this->btype=btype;
this->setIndex({0, 0});
this->setUserData(0);
bflags.store(0);
}

RigidBody::~RigidBody()
{
}

uint32 RigidBody::getBType()const
{
return btype;
}

void RigidBody::setVnum(uint32 vnum)
{
index.setVnum(vnum);
}

uint32 RigidBody::getVnum()const
{
return index.getVnum();
}

void RigidBody::setSubVnum(uint32 vnum)
{
index.setSubVnum(vnum);
}

uint32 RigidBody::getSubVnum()const
{
return index.getSubVnum();
}

void RigidBody::setIndex(const gpp_index& id)
{
this->index=id;
}

gpp_index RigidBody::getIndex()const
{
return index;
}

void RigidBody::setName(const string& name)
{
this->name=name;
}

string RigidBody::getName()const
{
return this->name;
}

void RigidBody::setUserData(uint32 udata)
{
this->userdata=udata;
}

uint32 RigidBody::getUserData()const
{
return this->userdata;
}

string RigidBody::toString()
{
stringstream ss;
ss<<fixed;
ss.precision(2);
ss<<"Index: "<<index.toString()<<endl;
ss<<"Nome: "<<this->getName()<<endl;
ss<<AABB::toString();
return ss.str();
}

void RigidBody::setBodyFlag(uint16 bf)
{
if((bflags&bf)!=bf)
{
bflags|=bf;
}
}

void RigidBody::removeBodyFlag(uint16 bf)
{
if((bflags&bf)!=0)
{
bflags^=bf;
}
}

bool RigidBody::containsBodyFlag(uint16 bf)const
{
return (bflags&bf)==bf;
}

bool RigidBody::isStatic()const
{
return containsBodyFlag(RB_STATIC);
}

bool RigidBody::isTransparent()const
{
return containsBodyFlag(RB_TRANSPARENT);
}

//geters e seters

    // Getters e setters
    float RigidBody::getMass() const { return mass; }
    void RigidBody::setMass(float m) { mass = m; }
    float RigidBody::getLinearDamping() const { return linearDamping; }
    void RigidBody::setLinearDamping(float ld) { linearDamping = ld; }
    float RigidBody::getAngularDamping() const { return angularDamping; }
    void RigidBody::setAngularDamping(float ad) { angularDamping = ad; }
    vector3d RigidBody::getPosition() const { return position; }
    void RigidBody::setPosition(const vector3d &p) { position = p; }
    vector3d RigidBody::getLinearMomentum() const { return linearMomentum; }
    void RigidBody::setLinearMomentum(const vector3d &lm) { linearMomentum = lm; }
    vector3d RigidBody::getAngularMomentum() const { return angularMomentum; }
    void RigidBody::setAngularMomentum(const vector3d &am) { angularMomentum = am; }
    vector3d RigidBody::getLinearVelocity() const { return linearVelocity; }
    void RigidBody::setLinearVelocity(const vector3d &lv) { linearVelocity = lv; }
    vector3d RigidBody::getAngularVelocity() const { return angularVelocity; }
    void RigidBody::setAngularVelocity(const vector3d &av) { angularVelocity = av; }
    vector3d RigidBody::getLinearAcceleration() const { return linearAcceleration; }
    void RigidBody::setLinearAcceleration(const vector3d &la) { linearAcceleration = la; }
    vector3d RigidBody::getAngularAcceleration() const { return angularAcceleration; }
    void RigidBody::setAngularAcceleration(const vector3d &aa) { angularAcceleration = aa; }
    vector3d RigidBody::getForce() const { return force; }
    void RigidBody::setForce(const vector3d &f) { force = f; }
    vector3d RigidBody::getTorque() const { return torque; }
    void RigidBody::setTorque(const vector3d &t) { torque = t; }
    matrix3x3 RigidBody::getInertiaTensor() const { return inertiaTensor; }
    void RigidBody::setInertiaTensor(const matrix3x3 &it) { inertiaTensor = it; }
    matrix3x3 RigidBody::getInverseInertiaTensor() const { return inverseInertiaTensor; }
void RigidBody::setInverseInertiaTensor(const matrix3x3& mt) {inverseInertiaTensor=mt;}
quaternion RigidBody::getOrientation()const { return orientation;}
void RigidBody::setOrientation(const quaternion& q) { orientation=q;}


//Funções extras...

void initRigidBody(RigidBody* rb)
{
    rb->setMass(1.0f);
    rb->setLinearDamping(0.0f);
    rb->setAngularDamping(0.0f);

    rb->setPosition(vector3d(0.0f, 0.0f, 0.0f));
    rb->setLinearMomentum(vector3d(0.0f, 0.0f, 0.0f));
    rb->setAngularMomentum(vector3d(0.0f, 0.0f, 0.0f));

    rb->setLinearVelocity(vector3d(0.0f, 0.0f, 0.0f));
    rb->setAngularVelocity(vector3d(0.0f, 0.0f, 0.0f));
    rb->setLinearAcceleration(vector3d(0.0f, 0.0f, 0.0f));
    rb->setAngularAcceleration(vector3d(0.0f, 0.0f, 0.0f));

    rb->setForce(vector3d(0.0f, 0.0f, 0.0f));
    rb->setTorque(vector3d(0.0f, 0.0f, 0.0f));

    matrix3x3 identityMatrix;
    identityMatrix.setIdentity();
    rb->setInertiaTensor(identityMatrix);
    rb->setInverseInertiaTensor(identityMatrix);

    rb->setOrientation(quaternion(0.0f, 0.0f, 0.0f, 1.0f));
}
}
