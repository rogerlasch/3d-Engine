
#include<sstream>
#include"types.h"
#include"gpp_index.h"
#include"vector3d.h"
#include"functions.h"
#include"geometry.h"
#include"RigidBody.h"

using namespace std;

namespace gpp
{
RigidBody::RigidBody()
{
this->setIndex({0, 0});
this->setPosition({0.0f, 0.0f, 0.0f});
this->setVelocity({0.0f, 0.0f, 0.0f});
this->setForse({0.0f, 0.0f, 0.0f});
this->setMass(0.0f);
bflags.reset();
}

RigidBody::~RigidBody()
{
}

void RigidBody::setName(const string& name)
{
this->name=name;
}

string RigidBody::getName()const
{
return this->name;
}

void RigidBody::setPosition(const vector3d& position)
{
this->position=position;
}

vector3d RigidBody::getPosition()const
{
return this->position;
}

void RigidBody::setBody(shape2d* body)
{
this->body=body;
}

shape2d* RigidBody::getBody()const
{
return this->body;
}

void RigidBody::setVelocity(const vector3d& velocity)
{
this->velocity=velocity;
}

vector3d RigidBody::getVelocity()const
{
return this->velocity;
}

void RigidBody::setForse(const vector3d& forse)
{
this->forse=forse;
}

vector3d RigidBody::getForse()const
{
return this->forse;
}

void RigidBody::setMass(float mass)
{
this->mass=mass;
}

float RigidBody::getMass()const
{
return this->mass;
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

void RigidBody::onStep(float dt, vector3d* onchanged)
{
if((dt<0.000001)||(mass<=0))
{
return;
}
vector3d v;
vector3d acceleration((forse/mass));
velocity+=(acceleration*dt);
v=velocity*dt;
position+=v;
}

string RigidBody::toString()
{
stringstream ss;
ss<<fixed;
ss.precision(2);
ss<<"Index: "<<index.toString()<<endl;
ss<<"Nome: "<<this->getName()<<endl;
ss<<"Position: "<<position.x<<" "<<position.y<<" "<<position.z<<endl;
ss<<"Velocity: "<<velocity.x<<" "<<velocity.y<<" "<<velocity.z<<endl;
ss<<"Forse: "<<forse.x<<" "<<forse.y<<" "<<forse.z<<endl;
ss<<"Mass: "<<mass<<endl;
return ss.str();
}

void RigidBody::translate(const vector3d& v)
{
position+=v;
if(body!=NULL)
{
body->translate(position);
}
}

void RigidBody::rotate(float angle, const vector3d& origin)
{
gpp_rotate_single_point(position, angle, origin);
}

void RigidBody::setBodyFlag(const initializer_list<uint32>& bflags)
{
for(auto& it : bflags)
{
this->bflags.set(it, true);
}
}

void RigidBody::removeBodyFlag(const initializer_list<uint32>& bflags)
{
for(auto& it : bflags)
{
this->bflags.set(it, false);
}
}

bool RigidBody::containsBodyFlag(const initializer_list<uint32>& bflags)const
{
for(auto& it : bflags)
{
if(this->bflags[it]==0)
{
return false;
}
}
return true;
}

bool RigidBody::isStatic()const
{
return bflags[RB_STATIC];
}

bool RigidBody::isTransparent()const
{
return bflags[RB_TRANSPARENT];
}
}
