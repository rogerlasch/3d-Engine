

#include<sstream>
#include"../common.h"
#include"../math/math.h"
#include"CollisionInfo.h"

using namespace std;

namespace gpp
{

CollisionInfo::CollisionInfo()
{
id=0;
point={0,0,0};
normal={0,0,0};
depth=0.0f;
r1=NULL;
r2=NULL;
}

CollisionInfo::CollisionInfo(const CollisionInfo& c)
{
*this=c;
}

CollisionInfo& CollisionInfo::operator=(const CollisionInfo& c)
{
this->id=c.id;
this->point=c.point;
this->normal=c.normal;
this->depth=c.depth;
this->r1=c.r1;
this->r2=c.r2;
return *this;
}

void CollisionInfo::reset()
{
id=0;
point={0,0,0};
normal={0,0,0};
depth=0.0f;
r1=NULL;
r2=NULL;
}

string CollisionInfo::toString()const
{
stringstream ss;
ss<<fixed;
ss.precision(2);
ss<<"Id: "<<id<<endl;
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
