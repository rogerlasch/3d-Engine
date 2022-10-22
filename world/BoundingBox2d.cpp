

#include"geometry.h"
#include"BoundingBox2d.h"

using namespace std;

namespace gpp
{
BoundingBox2d::BoundingBox2d()
{
this->reset();
}

BoundingBox2d::BoundingBox2d(const vector3d& m1, const vector3d& m2, const vector3d& m3, const vector3d& m4)
{
this->m1=m1;
this->m2=m2;
this->m3=m3;
this->m4=m4;
this->recalculate();
}

BoundingBox2d::BoundingBox2d(const BoundingBox2d& bb)
{
*this=bb;
}

BoundingBox2d& BoundingBox2d::operator=(const BoundingBox2d& bb)
{
this->m1=bb.m1;
this->m2=bb.m2;
this->m3=bb.m3;
this->m4=bb.m4;
this->center=bb.center;
this->measures=bb.measures;
return *this;
}

void BoundingBox2d::reset()
{
m1={0.0f, 0.0f, 0.0f};
m2={0.0f, 0.0f, 0.0f};
m3={0.0f, 0.0f, 0.0f};
m4={0.0f, 0.0f, 0.0f};
center={0.0f, 0.0f, 0.0f};
measures={0.0f, 0.0f, 0.0f};
}

void BoundingBox2d::recalculate()
{
measures.x=(m2.x-m1.x);
measures.y=(m4.y-m3.y);
center.x=(measures.x/2)+m1.x;
center.y=(measures.y/2)+m1.y;
}

bool BoundingBox2d::isColiding(BoundingBox2d* b)
{
vector3d v1=(this->m1-b->m4);
vector3d v2=((b->m1-this->m4));
if((v1.x>0.0f)||(v1.y>0.0f))
{
return false;
}
if((v2.x>0.0f)||(v2.y>0.0f))
{
return false;
}
return true;
}

bool BoundingBox2d::canContains(BoundingBox2d* b)
{
return ((this->measures.x*this->measures.y)>=(b->measures.x*b->measures.y));
}

bool BoundingBox2d::isContained(BoundingBox2d* b)
{
return ((this->m1.x<=b->m1.x)&&(this->m2.x>=b->m2.x)&&(this->m1.y<=b->m1.y)&&(this->m3.y>=b->m3.y));
}

void BoundingBox2d::translate(const vector3d& v)
{
m1+=v;
m2+=v;
m3+=v;
m4+=v;
center+=v;
}

void BoundingBox2d::scale(float sc)
{
m1*=sc;
m2*=sc;
m3*=sc;
m4*=sc;
center*=sc;
}

string BoundingBox2d::toString()const
{
stringstream ss;
ss<<fixed;
ss.precision(2);
ss<<"Limites: "<<m1.x<<":"<<m2.x<<", "<<m1.y<<":"<<m3.y<<endl;
return ss.str();
}
}
