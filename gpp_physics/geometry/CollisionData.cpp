

#include<sstream>
#include"vector3d.h"
#include"CollisionData.h"

using namespace std;

namespace gpp
{

CollisionData::CollisionData()
{
point={0,0,0};
normal={0,0,0};
depth=0.0f;
}

CollisionData::CollisionData(const CollisionData& c)
{
*this=c;
}

CollisionData& CollisionData::operator=(const CollisionData& c)
{
this->point=c.point;
this->normal=c.normal;
this->depth=c.depth;
return *this;
}

void CollisionData::reset()
{
point={0,0,0};
normal={0,0,0};
depth=0.0f;
}

string CollisionData::toString()const
{
stringstream ss;
ss<<"ponto: "<<point<<endl;
ss<<"normal: "<<normal<<endl;
ss<<"depth: "<<depth<<endl;
return ss.str();
}

ostream& operator<<(ostream& os, const CollisionData& cd)
{
os<<cd.toString();
return os;
}
}
