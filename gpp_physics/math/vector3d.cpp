
#include<sstream>
#include<vector>
#include"math.h"
#include"vector3d.h"


using namespace std;

namespace gpp
{

const vector3d vector3d::ORIGIN=vector3d(0.0f, 0.0f, 0.0f);
vector3d::vector3d(decimal x, decimal y, decimal z)
{
this->x=x;
this->y=y;
this->z=z;
}

vector3d::vector3d(const vector3d& dv)
{
*this=dv;
}

decimal& vector3d::operator[](unsigned int axis)
{
switch(axis)
{
case 0:
return this->x;
case 1:
return this->y;
case 2:
return this-> z;
default:
throw runtime_error("Invalid axis on vector3d");
}
}

decimal vector3d::operator[](unsigned int axis)const
{
switch(axis)
{
case 0:
return this->x;
case 1:
return this->y;
case 2:
return this-> z;
default:
throw runtime_error("Invalid axis on vector3d");
}
}

vector3d& vector3d::operator=(const vector3d& dv)
{
this->x=dv.x;
this->y=dv.y;
this->z=dv.z;
return *this;
}

vector3d& vector3d::operator=(const decimal points[3] )
{
/*
vector<decimal> df(points);
switch(df.size())
{
case 1:
{
this->x=df[0];
break;
}
case 2:
{
this->x=df[0];
this->y=df[1];
break;
}
case 3:
{
this->x=df[0];
this->y=df[1];
this->z=df[2];
break;
}
}
*/
x=points[0];
y=points[1];
z=points[2];
return *this;
}

bool vector3d::operator==(const vector3d& dv)const
{
return vector3dIsEqual(*this, dv, 0.1f);
}

bool vector3d::operator==(const decimal  dv[3])const
{
        vector3d v = { dv[0], dv[1], dv[2] };
return vector3dIsEqual(*this, v, 0.1f);
}

vector3d vector3d::operator-()
{
return vector3d(-x, -y, -z);
}

vector3d& vector3d::operator+=(decimal escalar)
{
this->x+=escalar;
this->y+=escalar;
this->z+=escalar;
return *this;
}

vector3d& vector3d::operator-=(decimal escalar)
{
this->x-=escalar;
this->y-=escalar;
this->z-=escalar;
return *this;
}

vector3d& vector3d::operator*=(decimal escalar)
{
this->x*=escalar;
this->y*=escalar;
this->z*=escalar;
return *this;
}

vector3d& vector3d::operator/=(decimal escalar)
{
if(escalar==0) escalar=1.0f;
this->x/=escalar;
this->y/=escalar;
this->z/=escalar;
return *this;
}

vector3d& vector3d::operator-=(const vector3d& dv)
{
this->x-=dv.x;
this->y-=dv.y;
this->z-=dv.z;
return *this;
}

vector3d& vector3d::operator+=(const vector3d& dv)
{
this->x+=dv.x;
this->y+=dv.y;
this->z+=dv.z;
return *this;
}

vector3d& vector3d::operator*=(const vector3d& dv)
{
this->x*=dv.x;
this->y*=dv.y;
this->z*=dv.z;
return *this;
}

vector3d& vector3d::operator/=(const vector3d& v )
{
if(v.x!=0)
{
this->x/=v.x;
}
if(v.y!=0)
{
this->y/=v.y;
}
if(v.z!=0)
{
this->z/=v.z;
}
return *this;
}

void vector3d::zero(){
this->x=0.0f;
this->y=0.0f;
this->z=0.0f;
}

decimal vector3d::length()const
{
return sqrt(x*x+y*y+z*z);
}

decimal vector3d::lengthSquared()const{
return ((x*x)+(y*y)+(z*z));
}

decimal vector3d::dot(const vector3d& v)const{
return vector3d::dot(*this, v);
}

vector3d vector3d::cross(const vector3d& v)const{
return vector3d::cross(*this, v);
}

vector3d& vector3d::normalize()
{
decimal len=sqrt(x*x+y*y+z*z);
if(len<=GPP_EPSILON) len=1.0f;
len=1.0f/len;
this->x*=len;
this->y*=len;
this->z*=len;
if(fabs(this->x)<=GPP_EPSILON) this->x=0.0f;
if(fabs(this->y)<=GPP_EPSILON) this->y=0.0f;
if(fabs(this->z)<=GPP_EPSILON) this->z=0.0f;
return *this;
}

vector3d& vector3d::inverse()
{
this->x=-this->x;
this->y=-this->y;
this->z=-this->z;
return *this;
}

vector3d vector3d::inverse(const vector3d& v)
{
return vector3d(-v.x, -v.y, -v.z);
}

//Útil quando precisamos manter o original intácto.
vector3d vector3d::normalize(const vector3d& v)
{
decimal len=sqrt(v.x*v.x+v.y*v.y+v.z*v.z);
if(len<=GPP_EPSILON) len=1.0f;
len=1.0f/len;
vector3d v2(v.x*len, v.y*len, v.z*len);
if(fabs(v2.x)<GPP_EPSILON) v2.x=0.0f;
if(fabs(v2.y)<GPP_EPSILON) v2.y=0.0f;
if(fabs(v2.z)<GPP_EPSILON) v2.z=0.0f;
return v2;
}

decimal vector3d::dot(const vector3d& v1, const vector3d&  v2)
{
return ((v1.x*v2.x)+(v1.y*v2.y)+(v1.z*v2.z));
}

vector3d vector3d::cross(const vector3d& v1, const vector3d& v2)
{
vector3d v;
v.x=v1.y*v2.z - v1.z*v2.y;
v.y=-v1.x*v2.z + v1.z*v2.x;
v.z= v1.x*v2.y - v1.y*v2.x;
return v;
}

decimal vector3d::triple(const vector3d& a, const vector3d& b, const vector3d& c)
{
//return a.x * (b.y * c.z - c.y * b.z) - a.y * (b.x * c.z - c.x * b.z) + a.z * (b.x * c.y - c.x * b.y);
return dot(cross(a, b), c);
}

string vector3d::toString()const
{
stringstream ss;
ss<<fixed;
ss<<x<<", "<<y<<", "<<z;
return ss.str();
}

//Functions

ostream& operator<<(ostream& os, const vector3d& dv)
{
os<<dv.x<<", "<<dv.y<<", "<<dv.z;
return os;
}

//overloads
//Aqui existem outras sobrecargas para operações entre vetores, bem como operadores de subtração adição, e etc...

vector3d operator+(const vector3d& v, decimal s)
{
return vector3d(v.x+s, v.y+s, v.z+s);
}

vector3d operator+(decimal s, const vector3d& v)
{
return vector3d(v.x+s, v.y+s, v.z+s);
}

vector3d operator-(const vector3d& v, decimal s)
{
return vector3d(v.x-s, v.y-s, v.z-s);
}

vector3d operator-(decimal s, const vector3d& v)
{
return vector3d(v.x-s, v.y-s, v.z-s);
}

vector3d operator*(const vector3d& v, decimal s)
{
return vector3d(v.x*s, v.y*s, v.z*s);
}

vector3d operator*(decimal s, const vector3d& v)
{
return vector3d(v.x*s, v.y*s, v.z*s);
}

vector3d operator/(const vector3d& v, decimal s)
{
if(s==0) s=1.0f;
return vector3d(v.x/s, v.y/s, v.z/s);
}

vector3d operator/(decimal s, const vector3d& v)
{
if(s==0) s=1.0f;
return vector3d(v.x/s, v.y/s, v.z/s);
}

vector3d operator+(const vector3d& v1, const vector3d& v2)
{
return vector3d(v1.x+v2.x, v1.y+v2.y, v1.z+v2.z);
}

vector3d operator-(const vector3d& v1, const vector3d& v2)
{
return vector3d(v1.x-v2.x, v1.y-v2.y, v1.z-v2.z);
}

vector3d multiVec(const vector3d& v1, const vector3d& v2)
{
return vector3d(v1.x*v2.x, v1.y*v2.y, v1.z*v2.z);
}

decimal operator*(const vector3d& v1, const vector3d& v2)
{
return((v1.x*v2.x)+(v1.y*v2.y)+(v1.z*v2.z));
}

vector3d operator/(const vector3d& v1, const vector3d& v2)
{
return vector3d(v1.x/((v2.x!=0) ? v2.x : 1.0f), v1.y/((v2.y!=0) ? v2.y : 1.0f), v1.z/((v2.z!=0) ? v2.z : 1.0f));
}

vector3d operator^(const vector3d& v1, const vector3d& v2)
{
vector3d v;
v.x=v1.y*v2.z - v1.z*v2.y;
v.y=-v1.x*v2.z + v1.z*v2.x;
v.z= v1.x*v2.y - v1.y*v2.x;
return v;
}

bool vector3dIsEqual(const vector3d& v1, const vector3d& v2, decimal tol){
vector3d v={fabs(v1.x-v2.x), fabs(v1.y-v2.y), fabs(v1.z-v2.z)};
return v.x<=tol&&v.y<=tol&&v.z<=tol;
}
}
