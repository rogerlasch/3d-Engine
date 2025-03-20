

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
    decimal len = this->length();
    if (len < 1e-8) return *this; // Evitar divisão por zero

    decimal invLen = 1.0f / len;
    x *= invLen;
    y *= invLen;
    z *= invLen;
    w *= invLen;

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

    quaternion normQ = *this;
    normQ.normalize();

    decimal xx = normQ.x * normQ.x;
    decimal xy = normQ.x * normQ.y;
    decimal xz = normQ.x * normQ.z;
    decimal xw = normQ.x * normQ.w;

    decimal yy = normQ.y * normQ.y;
    decimal yz = normQ.y * normQ.z;
    decimal yw = normQ.y * normQ.w;

    decimal zz = normQ.z * normQ.z;
    decimal zw = normQ.z * normQ.w;

    return matrix3x3{
{        1 - 2*(yy + zz), 2*(xy - zw),     2*(xz + yw)},
{        2*(xy + zw),     1 - 2*(xx + zz), 2*(yz - xw)},
{        2*(xz - yw),     2*(yz + xw),     1 - 2*(xx + yy)}
    };
}

matrix4x4 quaternion::toMatrix4x4() const {
    // Normaliza o quaternion para garantir que ele seja unitário
    quaternion normQ = *this;
    normQ.normalize();

    // Extrai os componentes do quaternion
    decimal x = normQ.x;
    decimal y = normQ.y;
    decimal z = normQ.z;
    decimal w = normQ.w;

    // Calcula os elementos da matriz 4x4
    decimal xx = x * x;
    decimal xy = x * y;
    decimal xz = x * z;
    decimal xw = x * w;

    decimal yy = y * y;
    decimal yz = y * z;
    decimal yw = y * w;

    decimal zz = z * z;
    decimal zw = z * w;

    // Constrói a matriz 4x4
    return matrix4x4{
        {1 - 2 * (yy + zz), 2 * (xy - zw),     2 * (xz + yw),     0},
        {2 * (xy + zw),     1 - 2 * (xx + zz), 2 * (yz - xw),     0},
        {2 * (xz - yw),     2 * (yz + xw),     1 - 2 * (xx + yy), 0},
        {0,                 0,                 0,                 1}
    };
}

quaternion quaternion::slerp(const quaternion& q1, const quaternion& q2, decimal t){

        // Garantir quaternions unitários
        quaternion from = quaternion::normalize(q1);
        quaternion to = quaternion::normalize(q2);

        decimal cosTheta = from.x*to.x + from.y*to.y + from.z*to.z + from.w*to.w;

        // Escolher o caminho mais curto
        if (cosTheta < 0.0f) {
            to = ~to;
            cosTheta = -cosTheta;
        }

        const decimal EPSILON = 1e-6f;
        if (cosTheta > 1.0f - EPSILON) {
            // Caso quase paralelo: usar LERP
            return lerp(from, to, t);
        }

        decimal theta = acos(cosTheta);
        decimal sinTheta = sin(theta);
        decimal a = sin((1 - t)*theta) / sinTheta;
        decimal b = sin(t*theta) / sinTheta;

        return from*a + to*b;

}

quaternion quaternion::lerp(const quaternion& q1, const quaternion& q2, decimal t) {
        return (q1*(1 - t) + q2*t).normalize();
    }

 quaternion quaternion::fromAxis(const vector3d& axis, decimal w){

    decimal halfAngle = w * 0.5f;
    decimal s = sin(halfAngle);
    vector3d normAxis = vector3d::normalize(axis);

    return quaternion(
        normAxis.x * s,
        normAxis.y * s,
        normAxis.z * s,
        cos(halfAngle)
    ).normalize();
}

 quaternion quaternion::fromEuler(decimal x, decimal y, decimal z){
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

quaternion quaternion::normalize(const quaternion& q) {
    decimal len = q.length();
    if (len < 1e-8) return q; // Evitar divisão por zero

    decimal invLen = 1.0f / len;
return quaternion(q.x * invLen,
    q.y * invLen,
    q.z * invLen,
    q.w * invLen);

    return q;
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

quaternion operator*(const quaternion& q1, const quaternion& q2) {
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

quaternion quaternion_rotate(const quaternion& q1, const quaternion& q2) {
return (q1*q2*(~q1));
}

vector3d quaternion_vector_rotate(const quaternion& q, const vector3d& v) {
/*
quaternion t;
t=((q*v)*~q);
return vector3d(t.x, t.y, t.z);
*/
    quaternion p(v.x, v.y, v.z, 0);
    quaternion result = q * p * q.conjugate();
    return vector3d(result.x, result.y, result.z);

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

/*
    vector3d angles;

    // Roll (x)
    decimal sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
    decimal cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);
    angles.x = radians_to_degrees(std::atan2(sinr_cosp, cosr_cosp));

    // Pitch (y)
    decimal sinp = 2 * (q.w * q.y - q.z * q.x);
    if (std::abs(sinp) >= 1)
        angles.y = std::copysign(GPP_PI / 2, sinp);
    else
        angles.y = radians_to_degrees(std::asin(sinp));

    // Yaw (z)
    decimal siny_cosp = 2 * (q.w * q.z + q.x * q.y);
    decimal cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
    angles.z =radians_to_degrees( std::atan2(siny_cosp, cosy_cosp));

    return angles;
*/
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
