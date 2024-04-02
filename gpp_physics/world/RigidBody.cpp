

#include"../math/math.h"
#include"../geometry/geometry.h"
#include"WorldProperties.h"
#include"BodyHandler.h"
#include"RigidBody.h"
#include"../octree/octree.h"

using namespace std;

namespace gpp{

RigidBody::RigidBody(){
hbody=NULL;
handler=new BodyHandler();
cleanup();
}

RigidBody::~RigidBody(){
cleanup();
delete handler;
handler=NULL;
}

string RigidBody::toString()const{
stringstream ss;
ss<<fixed;
ss.precision(2);
ss<<"Exibindo objeto "<<id<<endl;
ss<<"Id: "<<id<<", Restitution: "<<restitution<<endl;
ss<<"Mass: "<<mass<<", InvMass: "<<invMass<<endl;
ss<<"staticFriction: "<<staticFriction<<", dynamicFriction"<<dynamicFriction<<endl;
ss<<"Velocity: "<<velocity<<endl;
ss<<"Angular velocity: "<<angularVelocity<<endl;
ss<<"Forces: "<<forces<<endl;
ss<<"Torque: "<<torque<<endl;
ss<<hbody->toString()<<endl;
return ss.str();
}

void RigidBody::cleanup(){
id=0;
rbflags=0;
mass=0.0f;
invMass=0.0f;
restitution=0.0f;
staticFriction=0.0f;
dynamicFriction=0.0f;
userData=NULL;
hnode=NULL;
handler->toDefault();
if(hbody!=NULL)delete hbody;
hbody=NULL;
}

 void RigidBody::translate(const vector3d& ts){
if(hbody!=NULL&&mass>0.0f){
 hbody->translate(ts);
if(hnode!=NULL){
hnode->getOctree()->transiction(this);
}
}
}

 void RigidBody::rotate(const quaternion& orientation){
if(hbody!=NULL){
 hbody->rotate(orientation);
if(hnode!=NULL){
hnode->getOctree()->transiction(this);
}
}
}

 void RigidBody::rotate(const vector3d& origin, const quaternion& orientation){
if(hbody!=NULL) {
hbody->rotate(origin, orientation);
if(hnode!=NULL){
hnode->getOctree()->transiction(this);
}
}
}

void RigidBody::applyLinearImpulse(const vector3d& impulse){
if(mass>0.0f){
velocity+=impulse*invMass;
}
}
bool RigidBody::preStep(WorldProperties* props){
_GASSERT(props!=NULL);
addForce(props->gravity*mass);
return true;
}

void RigidBody::step(decimal dt){
if((mass<=0.0f)||(hbody==NULL))return;

//Movimento linear...

vector3d accel=forces*invMass;
velocity+=(accel*dt);

hbody->translate((velocity*dt));

//Orientação e movimento angular...
/*
updateInertiaTensor();
angularVelocity+=(invInertia*torque)*dt;
float adamping=powf(angularDamping, dt);
angularVelocity*=adamping;

quaternion q(1.0f, (angularVelocity*dt)*0.5f);
orientation=(orientation+q*orientation);
orientation.normalize();
*/

//Limpar as forças...
forces.zero();
torque.zero();

}

void RigidBody::onLoop(){
}

void RigidBody::beginContact(RigidBody* rb){
}

void RigidBody::updateContact(RigidBody* rb){
}

void RigidBody::endContact(RigidBody* rb){
}
}
