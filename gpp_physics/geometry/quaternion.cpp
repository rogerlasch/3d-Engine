

#include"vector3d.h"
#include"functions.h"
#include"quaternion.h"

using namespace std;

namespace gpp
{

quaternion::quaternion(float n, float x, float y, float z)
{
this->n=n;
v.x=x;
v.y=y;
v.z=z;
}

quaternion::quaternion(const quaternion& q)
{
*this=q;
}

quaternion& quaternion::operator=(const quaternion& q)
{
this->n=q.n;
this->v=q.v;
return *this;
}

quaternion& quaternion::operator+=(float s)
{
this->n+=s;
this->v+=s;
return *this;
}

quaternion& quaternion::operator-=(float s)
{
this->n-=s;
this->v-=s;
return *this;
}

quaternion& quaternion::operator*=(float s)
{
this->n*=s;
this->v*=s;
return *this;
}

quaternion& quaternion::operator/=(float s)
{
if(s==0) s=1;
this->n/=s;
this->v/=s;
return *this;
}

quaternion& quaternion::operator+=(const quaternion& q)
{
this->n+=q.n;
this->v+=q.v;
return *this;
}

quaternion& quaternion::operator-=(const quaternion& q)
{
this->n-=q.n;
this->v-=q.v;
return *this;
}

quaternion quaternion::operator~()const
{
return quaternion(n, -v.x, -v.y, -v.z);
}

void quaternion::setScalar(float n)
{
this->n=n;
}

float quaternion::getScalar()const
{
return this->n;
}

void quaternion::setVector(const vector3d& v)
{
this->v=v;
}

vector3d quaternion::getVector()const
{
return this->v;
}

float quaternion::length()
{
return sqrt(n*n+v.x*v.x+v.y*v.y+v.z*v.z);
}

quaternion quaternion::conjugate()const
{
return quaternion(n, -v.x, -v.y, -v.z);
}

float quaternion::getAngle()const
{
return (float)2*acos(n);
}

vector3d quaternion::getAxis()const
{
float m=v.length();
if(m<=GPP_VECTOR_TOLERANCE) return vector3d(0,0,0);
return v/m;
}



//Overloads

quaternion operator+(const quaternion& q, float s)
{
return quaternion(q.n+s, q.v.x+s, q.v.y+s, q.v.z+s);
}

quaternion operator+(float s, const quaternion& q)
{
return quaternion(q.n+s, q.v.x+s, q.v.y+s, q.v.z+s);
}

quaternion operator-(const quaternion& q, float s)
{
return quaternion(q.n-s, q.v.x-s, q.v.y-s, q.v.z-s);
}

quaternion operator-(float s, const quaternion& q)
{
return quaternion(q.n-s, q.v.x-s, q.v.y-s, q.v.z-s);
}

quaternion operator*(const quaternion& q, float s)
{
return quaternion(q.n*s, q.v.x*s, q.v.y*s, q.v.z*s);
}

quaternion operator*(float s, const quaternion& q)
{
return quaternion(q.n*s, q.v.x*s, q.v.y*s, q.v.z*s);
}

quaternion operator/(const quaternion& q, float s)
{
if(s==0) s=1.0f;
return quaternion(q.n/s, q.v.x/s, q.v.y/s, q.v.z/s);
}

quaternion operator/(float s, const quaternion& q)
{
if(s==0) s=0.0f;
return quaternion(q.n/s, q.v.x/s, q.v.y/s, q.v.z/s);
}

quaternion operator+(const quaternion& q1, const quaternion& q2)
{
return quaternion(q1.n+q2.n, q1.v.x+q2.v.y, q1.v.y+q2.v.y, q1.v.z+q2.v.z);
}

quaternion operator-(const quaternion& q1, const quaternion& q2)
{
return quaternion(q1.n-q2.n, q1.v.x-q2.v.y, q1.v.y-q2.v.y, q1.v.z-q2.v.z);
}

quaternion operator*(const quaternion& q1, const quaternion& q2)
{
return quaternion(q1.n*q2.n - q1.v.x*q2.v.x
 - q1.v.y*q2.v.y - q1.v.z*q2.v.z,
 q1.n *q2.v.x + q1.v.x*q2.n
 + q1.v.y*q2.v.z - q1.v.z*q2.v.y,
 q1.n*q2.v.y + q1 .v.y*q2.n
 + q1.v.z*q2.v.x - q1.v.x*q2.v.z,
 q1.n*q2.v.z + q1.v.z*q2.n
 + q1.v.x*q2.v.y - q1.v.y*q2.v.x);
}

quaternion operator/(const quaternion& q1, const quaternion& q2)
{
return quaternion(q1.n/q2.n, q1.v.x/q2.v.y, q1.v.y/q2.v.y, q1.v.z/q2.v.z);
}

quaternion operator*(const quaternion& q, const vector3d& v)
{
return quaternion( -(q.v.x*v.x + q.v.y*v.y + q.v.z*v.z),
 q.n*v.x + q.v.y*v.z - q.v.z*v.y,
 q.n*v.y + q.v.z*v.x - q.v.x*v.z,
 q.n*v.z + q.v.x*v.y - q.v.y*v.x);
}

quaternion operator*(const vector3d& v, quaternion& q)
{
return quaternion( -(q.v.x*v.x + q.v.y*v.y + q.v.z*v.z),
 q.n*v.x + q.v.y*v.z - q.v.z*v.y,
 q.n*v.y + q.v.z*v.x - q.v.x*v.z,
 q.n*v.z + q.v.x*v.y - q.v.y*v.x);
}

quaternion quaternion_rotate(const quaternion& q1, const quaternion& q2)
{
return q1*q2*(~q1);
}

vector3d quaternion_vector_rotate(const quaternion& q, const vector3d& v)
{
quaternion t;
t=(q*v)*~q;
return t.getVector();
}

quaternion quaternion_from_euler_angles(float x, float y, float z)
{
             quaternion q;
             double roll = degrees_to_radians(x);
             double pitch = degrees_to_radians(y);
             double yaw = degrees_to_radians(z);

double  cyaw, cpitch, croll, syaw, spitch, sroll;
double  cyawcpitch, syawspitch, cyawspitch, syawcpitch;

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

q.n = (float) (cyawcpitch * croll + syawspitch * sroll);
q.v.x = (float) (cyawcpitch * sroll - syawspitch * croll);
q.v.y = (float) (cyawspitch * croll + syawcpitch * sroll);
q.v.z = (float) (syawcpitch * croll - cyawspitch * sroll);
             return q;
}

vector3d quaternion_extract_euler_angles(const quaternion& q)
{
             double r11, r21, r31, r32, r33, r12, r13;
             double q00, q11, q22, q33;
             double tmp;
             vector3d u;
             q00 = q.n * q.n;
             q11 = q.v.x * q.v.x;
             q22 = q.v.y * q.v.y;
             q33 = q.v.z * q.v.z;
             r11 = q00 + q11 - q22 - q33;
             r21 = 2 * (q.v.x*q.v.y + q.n*q.v.z);
             r31 = 2 * (q.v.x*q.v.z - q.n*q.v.y);
r32 = 2 * (q.v.y*q.v.z + q.n*q.v.x);
r33 = q00 - q11 - q22 + q33;
tmp = fabs(r31);
if(tmp > 0.999999)
{
       r12 = 2 * (q.v.x*q.v.y - q.n*q.v.z);
       r13 = 2 * (q.v.x*q.v.z + q.n*q.v.y);
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
