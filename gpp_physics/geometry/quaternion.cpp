

#include<sstream>
#include"vector3d.h"
#include"functions.h"
#include"quaternion.h"

using namespace std;

namespace gpp
{

quaternion::quaternion(float x, float y, float z, float w)
{
this->x=vector3d::floatClean(x);
this->y=vector3d::floatClean(y);
this->z=vector3d::floatClean(z);
this->w=vector3d::floatClean(w);
}

quaternion::quaternion(const quaternion& q)
{
*this=q;
}

quaternion& quaternion::operator=(const quaternion& q)
{
this->x=vector3d::floatClean(q.x);
this->y=vector3d::floatClean(q.y);
this->z=vector3d::floatClean(q.z);
this->w=vector3d::floatClean(q.w);
return *this;
}

bool quaternion::operator==(const quaternion& q)
{
return this->w==q.w&&this->x==q.x&&this->y==q.y&&this->z==q.z;
}

quaternion& quaternion::operator+=(float s)
{
this->x+=s;
this->y+=s;
this->z+=s;
this->w+=s;
return *this;
}

quaternion& quaternion::operator-=(float s)
{
this->x-=s;
this->y-=s;
this->z-=s;
this->w-=s;
return *this;
}

quaternion& quaternion::operator*=(float s)
{
this->x*=s;
this->y*=s;
this->z*=s;
this->w*=s;
return *this;
}

quaternion& quaternion::operator/=(float s)
{
if(s==0) s=1;
this->x/=s;
this->y/=s;
this->z/=s;
this->w/=s;
return *this;
}

quaternion& quaternion::operator+=(const quaternion& q)
{
this->x+=q.x;
this->y+=q.y;
this->z+=q.z;
this->w+=q.w;
return *this;
}

quaternion& quaternion::operator-=(const quaternion& q)
{
this->x-=q.x;
this->y-=q.y;
this->z-=q.z;
this->w-=q.w;
return *this;
}

quaternion quaternion::operator~()const
{
return quaternion(-x, -y, -z, w);
}

void quaternion::setX(float x)
{
this->x=x;
}

float quaternion::getX()const
{
return this->x;
}

void quaternion::setY(float y)
{
this->y=y;
}

float quaternion::getY()const
{
return this->y;
}

void quaternion::setZ(float z)
{
this->z=z;
}

float quaternion::getZ()const
{
return this->z;
}

void quaternion::setW(float w)
{
this->w=w;
}

float quaternion::getW()const
{
return this->w;
}

float quaternion::length()const
{
return sqrt(((x*x)+(y*y)+(z*z)+(w*w)));
}

quaternion quaternion::conjugate()const
{
return quaternion(-x, -y, -z, w);
}

quaternion& quaternion::normalize()
{
    float len=this->length();
    if (len > 0) {
x/=len;
y/=len;
z/=len;
w/=len;
    }
return *this;
}

quaternion& quaternion::setIdentity()
{
x=0.0f;
y=0.0f;
z=0.0f;
w=1.0f;
return *this;
}

float quaternion::getAngle()const
{
return (float)2*acos(w);
}

vector3d quaternion::getAxis()const
{
float m=vector3d(x, y, z).length();
if(m<=GPP_VECTOR_TOLERANCE) return vector3d(0,0,0);
return vector3d(x/m, y/m, z/m);
}

string quaternion::toString() const
{
    stringstream result;
result<<x<<":"<<y<<":"<<z<<":"<<w;
    return result.str();
}


//Overloads

ostream& operator<<(ostream& os, const quaternion& q)
{
return os<<q.toString();
return os;
}

quaternion operator+(const quaternion& q, float s)
{
return quaternion(q.x+s, q.y+s, q.z+s, q.w+s);
}

quaternion operator+(float s, const quaternion& q)
{
return quaternion(q.x+s, q.y+s, q.z+s, q.w+s);
}

quaternion operator-(const quaternion& q, float s)
{
return quaternion(q.x-s, q.y-s, q.z-s, q.w-s);
}

quaternion operator-(float s, const quaternion& q)
{
return quaternion(q.x-s, q.y-s, q.z-s, q.w-s);
}

quaternion operator*(const quaternion& q, float s)
{
return quaternion(q.x*s, q.y*s, q.z*s, q.w*s);
}

quaternion operator*(float s, const quaternion& q)
{
return quaternion(q.x*s, q.y*s, q.z*s, q.w*s);
}

quaternion operator/(const quaternion& q, float s)
{
if(s==0) s=1.0f;
return quaternion(q.x/s, q.y/s, q.z/s, q.w/s);
}

quaternion operator/(float s, const quaternion& q)
{
if(s==0) s=0.0f;
return quaternion(q.x/s, q.y/s, q.z/s, q.w/s);
}

quaternion operator+(const quaternion& q1, const quaternion& q2)
{
return quaternion(q1.x+q2.x, q1.y+q2.y, q1.z+q2.z, q1.w+q2.w);
}

quaternion operator-(const quaternion& q1, const quaternion& q2)
{
return quaternion(q1.x-q2.x, q1.y-q2.y, q1.z-q2.z, q1.w-q2.w);
}

quaternion operator*(const quaternion& q1, const quaternion& q2)
{
/*
return quaternion(q1.w*q2.w - q1.x*q2.x
 - q1.y*q2.y - q1.z*q2.z,
 q1.w *q2.x + q1.x*q2.w
 + q1.y*q2.z - q1.z*q2.y,
 q1.w*q2.y + q1 .v.y*q2.w
 + q1.z*q2.x - q1.x*q2.z,
 q1.w*q2.z + q1.z*q2.w
 + q1.x*q2.y - q1.y*q2.x);
*/
  float w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;
  float x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y;
  float y = q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x;
  float z = q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w;
  return quaternion(x, y, z, w);
}

quaternion operator/(const quaternion& q1, const quaternion& q2)
{
return quaternion(q1.x/q2.y, q1.y/q2.y, q1.z/q2.z, q1.w/q2.w);
}

quaternion operator*(const quaternion& q, const vector3d& v)
{
float x=q.w*v.x + q.y*v.z - q.z*v.y;
float y= q.w*v.y + q.z*v.x - q.x*v.z;
float z= q.w*v.z + q.x*v.y - q.y*v.x;
float w=-(q.x*v.x + q.y*v.y + q.z*v.z);
return quaternion(x, y, z, w);
}

quaternion operator*(const vector3d& v, quaternion& q)
{
float x=q.w*v.x + q.y*v.z - q.z*v.y;
float y=q.w*v.y + q.z*v.x - q.x*v.z;
float z= q.w*v.z + q.x*v.y - q.y*v.x;
float w=-(q.x*v.x + q.y*v.y + q.z*v.z);
return quaternion(x, y, z, w);
}

quaternion quaternion_rotate(const quaternion& q1, const quaternion& q2)
{
return q1*q2*(~q1);
}

vector3d quaternion_vector_rotate(const quaternion& q, const vector3d& v)
{
quaternion t;
t=(q*v)*~q;
return vector3d(t.x, t.y, t.z);
}

quaternion quaternion_from_euler_angles(float x, float y, float z)
{
             double roll = degrees_to_radians(x);
             double pitch = degrees_to_radians(y);
             double yaw = degrees_to_radians(z);
double            cyaw, cpitch, croll, syaw, spitch, sroll;
double            cyawcpitch, syawspitch, cyawspitch, syawcpitch;

cyaw = cos(0.5f * yaw);
cpitch = cos(0.5f * pitch);
croll = cos(0.5f * roll);
syaw = sin(0.5f * yaw);
spitch = sin(0.5f * pitch);
sroll = sin(0.5f * roll);

cyawcpitch = cyaw*cpitch;
syawspitch = syaw*spitch;
cyawspitch = cyaw*spitch;
syawcpitch = syaw*cpitch;

float qx = (float) (cyawcpitch * sroll - syawspitch * croll);
float qy = (float) (cyawspitch * croll + syawcpitch * sroll);
float qz = (float) (syawcpitch * croll - cyawspitch * sroll);
float qw = (float) (cyawcpitch * croll + syawspitch * sroll);
             return quaternion(qx, qy, qz, qw);
}

vector3d quaternion_extract_euler_angles(const quaternion& q)
{
             double r11, r21, r31, r32, r33, r12, r13=0.00;
             double q00, q11, q22, q33=0.0;
             double tmp=0.00;
vector3d u;

q00 = q.w * q.w;
q11 = q.x * q.x;
q22 = q.y * q.y;
q33 = q.z * q.z;

r11 = q00 + q11 - q22 - q33;
r21 = 2 * (q.x*q.y + q.w*q.z);
r31 = 2 * (q.x*q.z - q.w*q.y);
r32 = 2 * (q.y*q.z + q.w*q.x);
r33 = q00 - q11 - q22 + q33;

tmp = fabs(r31);
if(tmp > 0.999999)
{

       r12 = 2 * (q.x*q.y - q.w*q.z);
       r13 = 2 * (q.x*q.z + q.w*q.y);

       u.x = radians_to_degrees(0.0f); //roll
       u.y = radians_to_degrees((float) (-(GPP_PI/2) * r31/tmp)); // pitch
       u.z = radians_to_degrees((float) atan2(-r12, -r31*r13)); // yaw
       return u;
}

u.x = radians_to_degrees((float) atan2(r32, r33)); // roll

u.y = radians_to_degrees((float) asin(-r31));  // pitch

u.z = radians_to_degrees((float) atan2(r21, r11)); // yaw

return u;
}
}
