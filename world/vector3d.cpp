


#include<vector>
#include<initializer_list>
#include"vector3d.h"


using namespace std;

namespace gpp
{
vector3d::vector3d(float x, float y, float z)
{
this->x=x;
this->y=y;
this->z=z;
}

vector3d::vector3d(const vector3d& dv)
{
*this=dv;
}

vector3d& vector3d::operator=(const vector3d& dv)
{
this->x=dv.x;
this->y=dv.y;
this->z=dv.z;
return *this;
}

vector3d& vector3d::operator=(const initializer_list<float>& points)
{
vector<float> df(points);
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
return *this;
}

bool vector3d::operator==(const vector3d& dv)const
{
return this->x==dv.x&&this->y==dv.y&&this->z==dv.z;
}

bool vector3d::operator==(const initializer_list<float>& dv)const
{
vector<float> df(dv);
switch(df.size())
{
case 1:
{
return this->x==df[0];
}
case 2:
{
return this->x==df[0]&&this->y==df[1];
}
case 3:
{
return this->x==df[0]&&this->y==df[1]&&this->z==df[2];
}
}
return false;
}

vector3d vector3d::operator-(const vector3d& dv)const
{
return vector3d(this->x-dv.x, this->y-dv.y, this->z-dv.z);
}

vector3d& vector3d::operator-=(const vector3d& dv)
{
this->x-=dv.x;
this->y-=dv.y;
this->z-=dv.z;
return *this;
}

vector3d vector3d::operator-(float escalar)
{
return vector3d(x-escalar, y-escalar, z-escalar);
}

vector3d& vector3d::operator-=(float escalar)
{
x-=escalar;
y-=escalar;
z-=escalar;
return *this;
}

vector3d vector3d::operator+(const vector3d& dv)
{
return vector3d(this->x+dv.x, this->y+dv.y, this->z=dv.z);
}

vector3d& vector3d::operator+=(const vector3d& dv)
{
this->x+=dv.x;
this->y+=dv.y;
this->z+=dv.z;
return *this;
}

vector3d vector3d::operator+(float escalar)
{
return vector3d(x+escalar, y+escalar, z+escalar);
}

vector3d& vector3d::operator+=(float escalar)
{
x+=escalar;
y+=escalar;
z+=escalar;
return *this;
}

vector3d vector3d::operator*(const vector3d& dv)
{
return vector3d(this->x*dv.x, this->y*dv.y, this->z*dv.z);
}

vector3d& vector3d::operator*=(const vector3d& dv)
{
this->x*=dv.x;
this->y*=dv.y;
this->z*=dv.z;
return *this;
}

vector3d vector3d::operator*(float escalar)
{
return vector3d(this->x*escalar, this->y*escalar, this->z*escalar);
}

vector3d& vector3d::operator*=(float escalar)
{
this->x*=escalar;
this->y*=escalar;
this->z*=escalar;
return *this;
}

vector3d vector3d::operator/(const vector3d& v )
{
return vector3d(((v.x!=0) ? this->x/v.x : this->x), ((v.y!=0) ? this->y/v.y : this->y), ((v.z!=0) ? this->z/v.z : this->z));
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

vector3d vector3d::operator/(float escalar)
{
if(escalar==0)
{
return vector3d(this->x, this->y, this->z);
}
return vector3d((this->x/escalar), (this->y/escalar), (this->z/escalar));
}

vector3d& vector3d::operator/=(float escalar)
{
if(escalar==0)
{
return *this;
}
this->x/=escalar;
this->y/=escalar;
this->z/=escalar;
return *this;
}

float vector3d::length()const
{
//is the same formula as the distance
return sqrt((this->x*this->x)+(this->y*this->y)+(this->z*this->z));
}

float vector3d::scalar_product(const vector3d& dv)
{
//just multiply everything and add.
return ((this->x*dv.x)+(this->y*dv.y)+(this->z*dv.z));
}

float vector3d::angle_from(const vector3d& dv)
{
float escalar=this->scalar_product(dv);
float product=(this->length()*dv.length());
float angle=acos((escalar/product));
angle=(angle*180)/GPP_PI;
return angle;
}

float vector3d::get_x()const
{
return this->x;
}

float vector3d::get_y()const
{
return this->y;
}

float vector3d::get_z()const
{
return this->z;
}

void vector3d::get_xyz(float * x, float* y, float* z)
{
if(x!=NULL)
{
*x=this->x;
}
if(y!=NULL)
{
*y=this->y;
}
if(z!=NULL)
{
*z=this->z;
}
}

void vector3d::set_x(float x)
{
this->x=x;
}

void vector3d::set_y(float y)
{
this->y=y;
}

void vector3d::set_z(float z)
{
this->z=z;
}

void vector3d::set_xyz(float x, float y, float z)
{
this->x=x;
this->y=y;
this->z=z;
}

float vector3d::get_distance(const vector3d& dv)const
{
float dx=(dv.x-this->x);
float dy=(dv.y-this->y);
float dz=(dv.z-this->z);
dx=(dx*dx);
dy=(dy*dy);
dz=(dz*dz);
float df=(dx+dy)+dz;
return sqrt(df);
}

float vector3d::get_squared_distance(const vector3d& v)const
{
vector3d v2=((*this)-v);
v2.x=v2.x*v2.x;
v2.y=v2.y*v2.y;
v2.z=v2.z*v2.z;
return ((v2.x+v2.y)+v2.z);
}

//Functions

ostream& operator<<(ostream& os, const vector3d& dv)
{
os.precision(2);
os<<fixed;
os<<dv.x<<"\t"<<dv.y<<"\t"<<dv.z<<endl;
return os;
}
}
