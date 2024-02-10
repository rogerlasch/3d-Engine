

#include<sstream>
#include"types.h"
#include"math/math.h"
#include"RigidBody.h"
#include"CollisionInfo.h"

using namespace std;

namespace gpp
{

CollisionInfo::CollisionInfo()
{
id=0;
counter=0;
point={0,0,0};
normal={0,0,0};
depth=0.0f;
r1=NULL;
r2=NULL;
notify=false;
}

CollisionInfo::CollisionInfo(const CollisionInfo& c)
{
*this=c;
}

CollisionInfo& CollisionInfo::operator=(const CollisionInfo& c)
{
this->counter=c.counter;
this->id=c.id;
this->point=c.point;
this->normal=c.normal;
this->depth=c.depth;
this->r1=c.r1;
this->r2=c.r2;
this->notify=c.notify;
return *this;
}

bool CollisionInfo::operator<(const CollisionInfo& info)const{
uint64 a1=(uint32)r1+(uint32)r2;
uint64 a2=(uint32)info.r1+(uint32)info.r2;
return a1 < a2;
}

uint64 CollisionInfo::getId()const{
return (uint64)r1+(uint64)r2;
}

void CollisionInfo::reset()
{
id=0;
counter=0;
point={0,0,0};
normal={0,0,0};
depth=0.0f;
r1=NULL;
r2=NULL;
notify=false;
}

string CollisionInfo::toString()const
{
stringstream ss;
ss<<fixed;
ss.precision(2);
ss<<"Id: "<<id<<endl;
ss<<"Notify: "<<boolalpha<<notify<<endl;
ss<<"ponto: "<<point<<endl;
ss<<"normal: "<<normal<<endl;
ss<<"depth: "<<depth<<endl;
return ss.str();
}

ostream& operator<<(ostream& os, const CollisionInfo& cd)
{
os<<"Id: "<<cd.id<<endl;
os<<"ponto: "<<cd.point<<endl;
os<<"normal: "<<cd.normal<<endl;
os<<"depth: "<<cd.depth<<endl;
return os;
}
}
