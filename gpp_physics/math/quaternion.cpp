

#include<sstream>
#include"math.h"
#include"quaternion.h"

using namespace std;

namespace gpp
{

quaternion::quaternion(decimal x, decimal y, decimal z, decimal w)
{
this->x=x;
this->y=y;
this->z=z;
this->w=w;
}

quaternion::quaternion(const vector3d& axis, decimal w)
{
this->x=axis.x;
this->y=axis.y;
this->z=axis.z;
this->w=w;
}

quaternion::quaternion(const quaternion& q)
{
*this=q;
}

quaternion& quaternion::operator=(const quaternion& q)
{
this->x=q.x;
this->y=q.y;
this->z=q.z;
this->w=q.w;
return *this;
}

bool quaternion::operator==(const quaternion& q)
{
return this->w==q.w&&this->x==q.x&&this->y==q.y&&this->z==q.z;
}

quaternion& quaternion::operator+=(decimal s)
{
this->x+=s;
this->y+=s;
this->z+=s;
this->w+=s;
return *this;
}

quaternion& quaternion::operator-=(decimal s)
{
this->x-=s;
this->y-=s;
this->z-=s;
this->w-=s;
return *this;
}

quaternion& quaternion::operator*=(decimal s)
{
this->x*=s;
this->y*=s;
this->z*=s;
this->w*=s;
return *this;
}

quaternion& quaternion::operator/=(decimal s)
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

void quaternion::setX(decimal x)
{
this->x=x;
}

decimal quaternion::getX()const
{
return this->x;
}

void quaternion::setY(decimal y)
{
this->y=y;
}

decimal quaternion::getY()const
{
return this->y;
}

void quaternion::setZ(decimal z)
{
this->z=z;
}

decimal quaternion::getZ()const
{
return this->z;
}

void quaternion::setW(decimal w)
{
this->w=w;
}

decimal quaternion::getW()const
{
return this->w;
}

decimal quaternion::length()const
{
return sqrt(((x*x)+(y*y)+(z*z)+(w*w)));
}

quaternion quaternion::conjugate()const
{
return quaternion(-x, -y, -z, w);
}

quaternion& quaternion::normalize()
{
    decimal len=this->length();
    if (len > 1.0f) {
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

decimal quaternion::getAngle()const
{
return (decimal)2*acos(w);
}

vector3d quaternion::getAxis()const
{
decimal m=sqrt((x*x)+(y*y)+(z*z));
vector3d v={x, y, z};
if(m>1.0f){
v/=m;
}
return v+numeric_limits<decimal>::epsilon();
}

string quaternion::toString() const
{
    stringstream result;
result<<fixed;
result.precision(2);
result<<x<<", "<<y<<", "<<z<<":"<<w;
    return result.str();
}

matrix3x3 quaternion::toMatrix3x3()const {
        decimal yy = y*y;
        decimal zz = z*z;
        decimal xy = x*y;
        decimal zw = z*w;
        decimal xz = x*z;
        decimal yw = y*w;
        decimal xx = x*x;
        decimal yz = y*z;
        decimal xw = x*w;

matrix3x3 mat;

        mat.e11 = 1 - 2 * yy - 2 * zz;
        mat.e12 = 2 * xy + 2 * zw;
        mat.e13 = 2 * xz - 2 * yw;

        mat.e21 = 2 * xy - 2 * zw;
        mat.e22 = 1 - 2 * xx - 2 * zz;
        mat.e23 = 2 * yz + 2 * xw;

        mat.e31 = 2 * xz + 2 * yw;
        mat.e32 = 2 * yz - 2 * xw;
        mat.e33 = 1 - 2 * xx - 2 * yy;
return mat;
}

 quaternion quaternion::fromAxis(const vector3d& axis, decimal w){
decimal       s = std::sin(w / 360.0f * GPP_PI);
vector3d v=vector3d::normalize(axis);
return quaternion(v*s, sqrt(1.0f - s * s));
}

 quaternion quaternion::fromEuler(decimal x, decimal y, decimal z){
             decimal roll = degrees_to_radians(x);
             decimal pitch = degrees_to_radians(y);
             decimal yaw = degrees_to_radians(z);
decimal            cyaw, cpitch, croll, syaw, spitch, sroll;
decimal            cyawcpitch, syawspitch, cyawspitch, syawcpitch;

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

decimal qx = (decimal) (cyawcpitch * sroll - syawspitch * croll);
decimal qy = (decimal) (cyawspitch * croll + syawcpitch * sroll);
decimal qz = (decimal) (syawcpitch * croll - cyawspitch * sroll);
decimal qw = (decimal) (cyawcpitch * croll + syawspitch * sroll);
             return quaternion(qx, qy, qz, qw);
}

//Overloads

ostream& operator<<(ostream& os, const quaternion& q)
{
return os<<q.x<<", "<<q.y<<", "<<q.z<<", "<<q.w;
return os;
}

quaternion operator+(const quaternion& q, decimal s)
{
return quaternion(q.x+s, q.y+s, q.z+s, q.w+s);
}

quaternion operator+(decimal s, const quaternion& q)
{
return quaternion(q.x+s, q.y+s, q.z+s, q.w+s);
}

quaternion operator-(const quaternion& q, decimal s)
{
return quaternion(q.x-s, q.y-s, q.z-s, q.w-s);
}

quaternion operator-(decimal s, const quaternion& q)
{
return quaternion(q.x-s, q.y-s, q.z-s, q.w-s);
}

quaternion operator*(const quaternion& q, decimal s)
{
return quaternion(q.x*s, q.y*s, q.z*s, q.w*s);
}

quaternion operator*(decimal s, const quaternion& q)
{
return quaternion(q.x*s, q.y*s, q.z*s, q.w*s);
}

quaternion operator/(const quaternion& q, decimal s)
{
if(s==0) s=1.0f;
return quaternion(q.x/s, q.y/s, q.z/s, q.w/s);
}

quaternion operator/(decimal s, const quaternion& q)
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
  decimal w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;
  decimal x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y;
  decimal y = q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x;
  decimal z = q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w;
  return quaternion(x, y, z, w);
}

quaternion operator/(const quaternion& q1, const quaternion& q2)
{
return quaternion(q1.x/q2.y, q1.y/q2.y, q1.z/q2.z, q1.w/q2.w);
}

quaternion operator*(const quaternion& q, const vector3d& v)
{
decimal x=q.w*v.x + q.y*v.z - q.z*v.y;
decimal y= q.w*v.y + q.z*v.x - q.x*v.z;
decimal z= q.w*v.z + q.x*v.y - q.y*v.x;
decimal w=-(q.x*v.x + q.y*v.y + q.z*v.z);
return quaternion(x, y, z, w);
}

quaternion operator*(const vector3d& v, quaternion& q)
{
decimal x=q.w*v.x + q.y*v.z - q.z*v.y;
decimal y=q.w*v.y + q.z*v.x - q.x*v.z;
decimal z= q.w*v.z + q.x*v.y - q.y*v.x;
decimal w=-(q.x*v.x + q.y*v.y + q.z*v.z);
return quaternion(x, y, z, w);
}

quaternion quaternion_rotate(const quaternion& q1, const quaternion& q2)
{
return (q1*q2*(~q1));
}

vector3d quaternion_vector_rotate(const quaternion& q, const vector3d& v)
{
quaternion t;
t=((q*v)*~q);
return vector3d(t.x, t.y, t.z);
}

quaternion quaternion_from_euler_angles(decimal x, decimal y, decimal z)
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

decimal qx = (decimal) (cyawcpitch * sroll - syawspitch * croll);
decimal qy = (decimal) (cyawspitch * croll + syawcpitch * sroll);
decimal qz = (decimal) (syawcpitch * croll - cyawspitch * sroll);
decimal qw = (decimal) (cyawcpitch * croll + syawspitch * sroll);
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
       u.y = radians_to_degrees((decimal) (-(GPP_PI/2) * r31/tmp)); // pitch
       u.z = radians_to_degrees((decimal) atan2(-r12, -r31*r13)); // yaw
       return u;
}

u.x = radians_to_degrees((decimal) atan2(r32, r33)); // roll

u.y = radians_to_degrees((decimal) asin(-r31));  // pitch

u.z = radians_to_degrees((decimal) atan2(r21, r11)); // yaw

return u;
}

quaternion quaternion_align_axis(const vector3d& v1, const vector3d& v2){
decimal eps=numeric_limits<decimal>::epsilon();
vector3d start=vector3d::normalize(v1);
vector3d dest=vector3d::normalize(v2);

decimal costheta=vector3d::dot(start, dest);

vector3d axis;

if(costheta<-1+0.001f){
axis=vector3d::cross({0.0f, 0.0f, 1.0f}, start);
if(vector3d::dot(axis, axis)<0.01){
axis=vector3d::cross({1.0f, 0.0f, 0.0f}, start);
}
axis.normalize();
return quaternion(axis, degrees_to_radians(180.0f))+eps;
}

axis=vector3d::cross(start, dest);

decimal s=sqrt((1+costheta)*2.0f);
decimal invs=1/s;

axis*=invs;

return quaternion(axis, s*0.5f)+eps;
}
}
