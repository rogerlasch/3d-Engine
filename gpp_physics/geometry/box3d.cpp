

#include"geometry.h"

using namespace std;

namespace gpp
{
box3d::box3d(const vector3d& min, const vector3d& measures):
GeometricShape(GTYPE_BOX)
{
this->min=min;
this->max=(min+measures);
}

box3d::box3d(const box3d& b):
GeometricShape(GTYPE_BOX)
{
*this=b;
}

box3d& box3d::operator=(const box3d& b)
{
this->min=b.min;
this->max=b.max;
return *this;
}

box3d:: ~box3d()
{
}

 vector3d box3d::getCenter()const
{
return (min+max)*0.5f;
}

void box3d::GetVertices(vector<vector3d>& vertices)
{
vertices.resize(0);
vertices.push_back(min);
vertices.push_back({min.x, max.y, min.z});
vertices.push_back({max.x, max.y, min.z});
vertices.push_back({max.x, min.y, min.z});
vertices.push_back({min.x, min.y, max.z});
vertices.push_back({min.x, max.y, max.z});
vertices.push_back(max);
vertices.push_back({max.x, min.y, max.z});
}

   void box3d::translate(const vector3d& translation)
{
setLastPosition(getCenter());
min+=translation;
max+=translation;
}

   void box3d::scale(float s)
{
vector3d center=getCenter();
min=(min*s)+center;
max=(max*s)+center;
}

 void box3d::scale(const vector3d& sc)
{
vector3d center=getCenter();
for(uint32 i=0; i<3; i++)
{
min[i]*=sc[i];
max[i]*=sc[i];
}
min+=center;
max+=center;
}

void box3d::rotate(const quaternion& orientation)
{
min=quaternion_vector_rotate(orientation, min);
max=quaternion_vector_rotate(orientation, min);
for(uint32 i=0; i<3; i++)
{
if(max[i]<min[i])
{
swap(min[i], max[i]);
}
}
}

matrix3x3 box3d::getInertiaTensor(float mass) const
{
    const float one_twelfth = 1.0f / 12.0f;
    matrix3x3 inertiaTensor;
vector3d measures=(max-min);
    float x = measures.x, y = measures.y, z = measures.z;
    vector3d v;
    v.x = mass * (y*y + z*z) * one_twelfth;
    v.y = mass * (x*x + z*z) * one_twelfth;
    v.z = mass * (x*x + y*y) * one_twelfth;
    inertiaTensor.setDiagonal(v);
    return inertiaTensor;
}
}
