

#include<sstream>
#include"vector3d.h"
#include"CollisionInfo.h"

using namespace std;

namespace gpp
{

CollisionInfo::CollisionInfo()
{
point={0,0,0};
normal={0,0,0};
depth=0.0f;
b1=NULL;
b2=NULL;
}

CollisionInfo::CollisionInfo(const CollisionInfo& c)
{
*this=c;
}

CollisionInfo& CollisionInfo::operator=(const CollisionInfo& c)
{
this->point=c.point;
this->normal=c.normal;
this->depth=c.depth;
this->b1=c.b1;
this->b2=c.b2;
return *this;
}

void CollisionInfo::reset()
{
point={0,0,0};
normal={0,0,0};
depth=0.0f;
b1=NULL;
b2=NULL;
}

string CollisionInfo::toString()const
{
stringstream ss;
ss<<"ponto: "<<point<<endl;
ss<<"normal: "<<normal<<endl;
ss<<"depth: "<<depth<<endl;
return ss.str();
}

ostream& operator<<(ostream& os, const CollisionInfo& cd)
{
os<<cd.toString();
return os;
}
}
