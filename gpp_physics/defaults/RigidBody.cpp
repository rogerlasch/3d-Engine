
#include"../gpp_physics.h"
#include<sstream>
#include<bitset>
#include"RigidBody.h"

using namespace std;

namespace gpp
{
RigidBody::RigidBody(uint32 btype)
{
this->setBType(btype);
this->setIndex({0, 0});
this->setUserData(0);
bflags.store(0);
aabb=new AABB();
contacts.store(0);
}

RigidBody::~RigidBody()
{
if(aabb!=NULL)
{
delete aabb;
}
}

string RigidBody::toString()const
{
stringstream ss;
ss<<fixed;
ss.precision(2);
ss<<"Index: "<<index.toString()<<endl;
ss<<"BType: "<<getBType()<<endl;
ss<<"BFlags: "<<bflags.load()<<endl;
ss<<"Nome: "<<this->getName()<<endl;
ss<< "Mass: "<<mass<<endl;//Maça do corpo
ss<<"Restitution: "<<restitution<<endl;
ss<< "position: "<<position<<endl;//Posição do corpo...
ss<<"Velocity: "<<velocity<<endl;
ss<< "orientation: "<<orientation<<endl;
ss<<aabb->toString();
return ss.str();
}
}
