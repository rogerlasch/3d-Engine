


#include"dlb_geometry.h"
#include"dlb_vector.h"


using namespace std;

namespace dlb
{
dlb_vector::dlb_vector(float x, float y, float z)
{
this->x=x;
this->y=y;
this->z=z;
}

dlb_vector::dlb_vector(const dlb_vector& dv)
{
*this=dv;
}

dlb_vector& dlb_vector::operator=(const dlb_vector& dv)
{
this->x=dv.x;
this->y=dv.y;
this->z=dv.z;
return *this;
}

dlb_vector& dlb_vector::operator=(const initializer_list<float>& points)
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

bool dlb_vector::operator==(const dlb_vector& dv)const
{
return this->x==dv.x&&this->y==dv.y&&this->z==dv.z;
}

bool dlb_vector::operator==(const initializer_list<float>& dv)const
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

dlb_vector dlb_vector::operator-(const dlb_vector& dv)
{
return dlb_vector(this->x-dv.x, this->y-dv.y, this->z-dv.z);
}

dlb_vector dlb_vector::operator-=(const dlb_vector& dv)
{
this->x-=dv.x;
this->y-=dv.y;
this->z-=dv.z;
return *this;
}

dlb_vector dlb_vector::operator+(const dlb_vector& dv)
{
return dlb_vector(this->x+dv.x, this->y+dv.y, this->z=dv.z);
}

dlb_vector dlb_vector::operator+=(const dlb_vector& dv)
{
this->x+=dv.x;
this->y+=dv.y;
this->z+=dv.z;
return *this;
}

dlb_vector dlb_vector::operator*(const dlb_vector& dv)
{
return dlb_vector(this->x*dv.x, this->y*dv.y, this->z*dv.z);
}

dlb_vector dlb_vector::operator*=(const dlb_vector& dv)
{
this->x*=dv.x;
this->y*=dv.y;
this->z*=dv.z;
return *this;
}

dlb_vector dlb_vector::operator*(float escalar)
{
return dlb_vector(this->x*escalar, this->y*escalar, this->z*escalar);
}

dlb_vector dlb_vector::operator*=(float escalar)
{
this->x*=escalar;
this->y*=escalar;
this->z*=escalar;
return *this;
}

float dlb_vector::length()const
{
//is the same formula as the distance
return sqrt((this->x*this->x)+(this->y*this->y)+(this->z*this->z));
}

float dlb_vector::scalar_product(const dlb_vector& dv)
{
//just multiply everything and add.
return ((this->x*dv.x)+(this->y*dv.y)+(this->z*dv.z));
}

float dlb_vector::angle_from(const dlb_vector& dv)
{
float escalar=this->scalar_product(dv);
float product=(this->length()*dv.length());
float angle=acos((escalar/product));
angle=(angle*180)/dlb_pi;
return angle;
}

float dlb_vector::get_x()const
{
return this->x;
}

float dlb_vector::get_y()const
{
return this->y;
}

float dlb_vector::get_z()const
{
return this->z;
}

void dlb_vector::get_xyz(float * x, float* y, float* z)
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

void dlb_vector::set_x(float x)
{
this->x=x;
}

void dlb_vector::set_y(float y)
{
this->y=y;
}

void dlb_vector::set_z(float z)
{
this->z=z;
}

void dlb_vector::set_xyz(float x, float y, float z)
{
this->x=x;
this->y=y;
this->z=z;
}

float dlb_vector::get_1d_distance(const dlb_vector& dv, int axis)const
{
switch(axis)
{
case ax_x:
{
return abs(this->x-dv.x);
}
case ax_y:
{
return abs(this->y-dv.y);
}
case ax_z:
{
return abs(this->z-dv.z);
}
}
return -1;
}

 float dlb_vector::get_2d_distance(const dlb_vector& dv)const
{
float dx=(abs(dv.x-this->x));
float dy=(abs(dv.y-this->y));
dx=(dx*dx);
dy=(dy*dy);
float df=(dx+dy);
return sqrt(df);
}

float dlb_vector::get_3d_distance(const dlb_vector& dv)const
{
float dx=(abs(dv.x-this->x));
float dy=(abs(dv.y-this->y));
float dz=(abs(dv.z-this->z));
dx=(dx*dx);
dy=(dy*dy);
dz=(dz*dz);
float df=(dx+dy)+dz;
return sqrt(df);
}

dlb_vector dlb_vector::get_distance(const dlb_vector& dv, const initializer_list<int>& axis)
{
dlb_vector df(-1, -1, -1);
for(auto it=axis.begin(); it!=axis.end(); ++it)
{
switch((*it))
{
case ax_x:
{
df.x=abs(dv.x-this->x);
break;
}
case ax_y:
{
df.y=abs(dv.y-this->y);
break;
}
case ax_z:
{
df.z=abs(dv.z-this->z);
break;
}
}
}
return df;
}

//Functions

ostream& operator<<(ostream& os, const dlb_vector& dv)
{
os.precision(2);
os<<fixed;
os<<dv.x<<"\t"<<dv.y<<"\t"<<dv.z<<endl;
return os;
}


}
