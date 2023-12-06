
#include<sstream>
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

string box3d::toString()const
{
stringstream ss;
ss<<fixed;
ss.precision(2);
vector<string> achis={"X", "Y", "Z"};
ss<<"Tipo: "<<getGeometricType()<<": Caixa"<<endl;
for(uint32 i=0; i<3; i++)
{
ss<<safe_format("{}: {:.2f}:{:.2f}", achis[i], min[i], max[i])<<endl;
}
return ss.str();
}

float box3d::getProjectedArea()const{
vector3d v=max-min;
return v.x*v.y;
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

   void box3d::translate(const vector3d& translation, bool correction)
{
if(!correction){
setLastPosition(getCenter());
}
min+=translation;
max+=translation;
}

   void box3d::scale(const vector3d& origin, float s)
{
min-=origin;
max-=origin;
min*=s;
max*=s;
min+=origin;
max+=origin;
}

 void box3d::scale(const vector3d& origin, const vector3d& sc)
{
min-=origin;
max-=origin;
for(uint32 i=0; i<3; i++)
{
min[i]*=sc[i];
max[i]*=sc[i];
}
min+=origin;
max+=origin;
}

void box3d::rotate(const vector3d& origin, const quaternion& orientation)
{
min-=origin;
max-=origin;
min=quaternion_vector_rotate(orientation, min);
max=quaternion_vector_rotate(orientation, max);
min+=origin;
max+=origin;
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
