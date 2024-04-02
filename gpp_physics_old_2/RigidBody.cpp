
#include<sstream>
#include<gpp/debug_system.h>
#include"math/math.h"
#include"CollisionInfo.h"
#include"RigidBody.h"
#include"fFunctions.h"

using namespace std;

namespace gpp{

uint32 refcount=0;

RigidBody::RigidBody(){
refcount++;
id=refcount;
_GINFO("Construiu id {}", id);
restitution=0.5f;
staticFriction=0.2f;
dynamicFriction=0.25f;
mass=2.0f;
inverseMass=1/mass;
lastPosition={0,0,0};
position={0,0,0};
velocity={0,0,0};
angularVelocity={0,0,0};
forces={0,0,0};
torque={0,0,0};
orientation=quaternion_from_euler_angles(0, 0, 0);
vector3d vinertia={1.0f/6.0f, 1.0f/6.0f, 1.0f/6.0f};
inertia.setDiagonal(vinertia);
invInertia = inertia.transpose();
rbflags=0;
hsurface=NULL;
}

RigidBody::~RigidBody(){
}

 void RigidBody::cleanup(){
id=refcount;

restitution=0.5f;
mass=2.0f;
inverseMass=1/mass;
lastPosition={0,0,0};
position={0,0,0};
velocity={0,0,0};
angularVelocity={0,0,0};
forces={0,0,0};
torque={0,0,0};
orientation=quaternion_from_euler_angles(0, 0, 0);
inertia.setDiagonal({1/6, 1/6, 1/6});
invInertia = inertia.transpose();
rbflags=0;
}

 string RigidBody::toString()const{

stringstream ss;
ss<<fixed;
ss.precision(2);
ss<<"Exibindo objeto "<<id<<endl;
ss<<"Last Position: "<<lastPosition<<endl;
ss<<"Position: "<<position<<endl;
ss<<"Velocity: "<<velocity<<endl;
ss<<"Angular velocity: "<<angularVelocity<<endl;
ss<<"Forces: "<<forces<<endl;
ss<<"Torque: "<<torque<<endl;
ss<<"Orientation: "<<orientation<<endl;
ss<<"Euler orientation: "<<quaternion_extract_euler_angles(orientation)<<endl;
ss<<"Inertia: "<<inertia<<endl;
ss<<"Inverse Inertia: "<<invInertia<<endl;
return ss.str();
}

vector3d RigidBody::getVelocityAtPoint(const vector3d& point){
vector3d dir=point-position;
vector3d angv=angularVelocity^dir;
return velocity+angv;
}
 void RigidBody::addTorque(const vector3d& tr, const vector3d& point){
addForce(tr);
if(mass>0.0f){
vector3d localpos=point-position;
torque+=localpos^tr;
}
}

void RigidBody::applyLinearImpulse(const vector3d& impulse){
if(mass>0.0f){
velocity+=impulse*inverseMass;
}
}

void RigidBody::applyAngularImpulse(const vector3d& impulse, const vector3d& point){
if(mass>0.0f){
velocity+=inverseMass*impulse;
angularVelocity+=invInertia*vector3d::crossProduct(point-position, impulse);
}
}

void RigidBody::updateInertiaTensor() {
matrix3x3 mt=orientation.toMatrix3x3();
matrix3x3 invmt=orientation.conjugate().toMatrix3x3();

matrix3x3 interm;
interm.setDiagonal(vecInertia);
invInertia=mt*interm*invmt;
}

void RigidBody::calculateInertia(){
}

void RigidBody::prepareBody(float dt, const vector3d& gravity){
vector3d fTotalForce;
fTotalForce+=gravity*mass;
addForce(fTotalForce);
}

 void RigidBody::beginContact(RigidBody* rb, CollisionInfo* info){
//_GINFO("{} entrou em contato com {} pela primeira vez...", id, rb->id);
//vector3d v=quaternion_extract_euler_angles(orientation);
//_GINFO("{:.2f}, {:.2f}, {:.2f}", v.x, v.y, v.z);
this->hsurface=rb;
}

 void RigidBody::endContact(RigidBody* rb, CollisionInfo* info){
//_GINFO("{} não tem mais contato com {}", id, rb->id);
if(hsurface==rb){
this->hsurface=NULL;
}
}

float RigidBody::getAabb(vector3d& min, vector3d& max)const{
min=position;
max=position;
return 1.0f;
}
}
