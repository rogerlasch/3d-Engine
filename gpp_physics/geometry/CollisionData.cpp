

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
}
