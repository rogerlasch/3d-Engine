
#include<sstream>
#include<bitset>
#include"../gpp/gpp.h"
#include"types.h"
#include"geometry/geometry.h"
#include"RigidBody.h"

using namespace std;

namespace gpp
{
RigidBody::RigidBody(uint32 btype):
AABB()
{
this->btype=btype;
this->setIndex({0, 0});
this->setVelocity({0.0f, 0.0f, 0.0f});
this->setForse({0.0f, 0.0f, 0.0f});
this->setMass(0.0f);
this->setUserData(0);
bflags.replace_flags(0);
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

vector3d RigidBody::nextStep(float dt)
{
if((dt<0.000001)||(mass<=0))
{
return vector3d();
}
vector3d v;
vector3d acceleration((forse/mass));
velocity+=(acceleration*dt);
v=velocity*dt;
return v;
}


string RigidBody::toString()
{
stringstream ss;
ss<<fixed;
ss.precision(2);
ss<<"Index: "<<index.toString()<<endl;
ss<<"Nome: "<<this->getName()<<endl;
ss<<"Velocity: "<<velocity.x<<" "<<velocity.y<<" "<<velocity.z<<endl;
ss<<"Forse: "<<forse.x<<" "<<forse.y<<" "<<forse.z<<endl;
ss<<"Mass: "<<mass<<endl;
ss<<AABB::toString();
return ss.str();
}

void RigidBody::setBodyFlag(uint16 bflags)
{
this->bflags.setflag(bflags);
}

void RigidBody::removeBodyFlag(uint16 bflags)
{
this->bflags.removeflag(bflags);
}

bool RigidBody::containsBodyFlag(uint16 bflags)
{
return this->bflags.flag_contains(bflags);
}

bool RigidBody::isStatic()const
{
return bflags.flag_contains(RB_STATIC);
}

bool RigidBody::isTransparent()const
{
return bflags.flag_contains(RB_TRANSPARENT);
}
}
