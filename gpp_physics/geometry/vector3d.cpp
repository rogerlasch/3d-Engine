


#include<sstream>
#include<vector>
#include<initializer_list>
#include"vector3d.h"


using namespace std;

namespace gpp
{
vector3d::vector3d(float x, float y, float z)
{
this->x=floatClean(x);
this->y=floatClean(y);
this->z=floatClean(z);
}

vector3d::vector3d(const vector3d& dv)
{
*this=dv;
}

float& vector3d::operator[](unsigned int axis)
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

float vector3d::operator[](unsigned int axis)const
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
this->x=floatClean(dv.x);
this->y=floatClean(dv.y);
this->z=floatClean(dv.z);
return *this;
}

vector3d& vector3d::operator=(const initializer_list<float>& points)
{
vector<float> df(points);
switch(df.size())
{
case 1:
{
this->x=floatClean(df[0]);
break;
}
case 2:
{
this->x=floatClean(df[0]);
this->y=floatClean(df[1]);
break;
}
case 3:
{
this->x=floatClean(df[0]);
this->y=floatClean(df[1]);
this->z=floatClean(df[2]);
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

vector3d& vector3d::operator-()
{
this->x=floatClean(-x);
this->y=floatClean(-y);
this->z=floatClean(-z);
return *this;
}

vector3d& vector3d::operator+=(float escalar)
{
this->x+=escalar;
this->y+=escalar;
this->z+=escalar;
return *this;
}

vector3d& vector3d::operator-=(float escalar)
{
this->x-=escalar;
this->y-=escalar;
this->z-=escalar;
return *this;
}

vector3d& vector3d::operator*=(float escalar)
{
this->x*=escalar;
this->y*=escalar;
this->z*=escalar;
return *this;
}

vector3d& vector3d::operator/=(float escalar)
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

bool vector3d::isNull()const
{
return x==0.0f&&y==0.0f&&z==0.0f;
}

//Calcula a magnitude do vetor, ou comprimento.
float vector3d::length()const
{
return sqrt(x*x+y*y+z*z);
}

//Normaliza o vetor...
vector3d& vector3d::normalize()
{
float len=this->length();
if(len<=numeric_limits<float>::epsilon()) len=1.0f;
this->x/=len;
this->y/=len;
this->z/=len;
if(fabs(this->x)<numeric_limits<float>::epsilon()) this->x=0.0f;
if(fabs(this->y)<numeric_limits<float>::epsilon()) this->y=0.0f;
if(fabs(this->z)<numeric_limits<float>::epsilon()) this->z=0.0f;
return *this;
}

//Inverte o vetor...
vector3d& vector3d::inverse()
{
this->x=floatClean(-this->x);
this->y=floatClean(-this->y);
this->z=floatClean(-this->z);
return *this;
}

//Inverte o vetor retornando um novo vetor.
vector3d vector3d::inverse(const vector3d& v)
{
return vector3d(-v.x, -v.y, -v.z);
}

//recupera uma das 3 coordenadas possíveis...

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

//Normaliza o vetor, versão estática.
//Útil quando precisamos manter o original intácto.
vector3d vector3d::normalize(const vector3d& v)
{
float len=v.length();
if(len<=numeric_limits<float>::epsilon()) len=1.0f;
vector3d v2(v.x/len, v.y/len, v.z/len);
if(fabs(v2.x)<numeric_limits<float>::epsilon()) v2.x=0.0f;
if(fabs(v2.y)<numeric_limits<float>::epsilon()) v2.y=0.0f;
if(fabs(v2.z)<numeric_limits<float>::epsilon()) v2.z=0.0f;
return v2;
}

//Calcula o produto entre 2 vetores...
float vector3d::dotProduct(const vector3d& v1, const vector3d&  v2)
{
//just multiply everything and add.
return ((v1.x*v2.x)+(v1.y*v2.y)+(v1.z*v2.z));
}

//Calcula o produto cruzado entre 2 vetores...
vector3d vector3d::crozProduct(const vector3d& v1, const vector3d& v2)
{
vector3d v;
v.x=v1.y*v2.z - v1.z*v2.y;
v.y=-v1.x*v2.z + v1.z*v2.x;
v.z= v1.x*v2.y - v1.y*v2.x;
return v;
}

//Calcula o produto triplo entre 3 vetores...
float vector3d::tripleProduct(const vector3d& a, const vector3d& b, const vector3d& c)
{
return a.x * (b.y * c.z - c.y * b.z) - a.y * (b.x * c.z - c.x * b.z) + a.z * (b.x * c.y - c.x * b.y);

}

//Calcula o ângulo entre 2 vetores...
float vector3d::angle_from(const vector3d&  v1, const vector3d&  v2)
{
float escalar=dotProduct(v1, v2);
float product=(v1.length()*v2.length());
float angle=acos((escalar/product));
angle=(angle*180)/GPP_PI;
return angle;
}

//Calcula a distância entre 2 vetores...
float vector3d::get_distance(const vector3d&  v1, const vector3d&  v2)
{
vector3d v(v1-v2);
v.x*=v.x;
v.y*=v.y;
v.z*=v.z;
float df=(v.x+v.y+v.z);
return sqrt(df);
}

//Calcula a distância ao quadrado entre 2 vetores...
float vector3d::get_squared_distance(const vector3d&  v1, const vector3d&  v2)
{
vector3d v(v1-v2);
v.x*=v.x;
v.y*=v.y;
v.z*=v.z;
float df=(v.x+v.y+v.z);
return df;
}

float vector3d::floatClean(float v)
{
if(isnan(v))
{
return 0.0f;
}
if(isinf(v))
{
return 0.0f;
}
if(fabs(v)<numeric_limits<float>::epsilon())
{
return 0.0f;
}
return v;
}

string vector3d::toString()const
{
stringstream ss;
ss<<x<<":"<<y<<":"<<z;
return ss.str();
}

//Functions

ostream& operator<<(ostream& os, const vector3d& dv)
{
os<<dv.toString();
return os;
}

//overloads
//Aqui existem outras sobrecargas para operações entre vetores, bem como operadores de subtração adição, e etc...

vector3d operator+(const vector3d& v, float s)
{
return vector3d(v.x+s, v.y+s, v.z+s);
}

vector3d operator+(float s, const vector3d& v)
{
return vector3d(v.x+s, v.y+s, v.z+s);
}

vector3d operator-(const vector3d& v, float s)
{
return vector3d(v.x-s, v.y-s, v.z-s);
}

vector3d operator-(float s, const vector3d& v)
{
return vector3d(v.x-s, v.y-s, v.z-s);
}

vector3d operator*(const vector3d& v, float s)
{
return vector3d(v.x*s, v.y*s, v.z*s);
}

vector3d operator*(float s, const vector3d& v)
{
return vector3d(v.x*s, v.y*s, v.z*s);
}

vector3d operator/(const vector3d& v, float s)
{
if(s==0) s=1.0f;
return vector3d(v.x/s, v.y/s, v.z/s);
}

vector3d operator/(float s, const vector3d& v)
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

vector3d operator*(const vector3d& v1, const vector3d& v2)
{
return vector3d(v1.x*v2.x, v1.y*v2.y, v1.z*v2.z);
}

vector3d operator/(const vector3d& v1, const vector3d& v2)
{
return vector3d(v1.x/((v2.x!=0) ? v2.x : 1.0f), v1.y/((v2.y!=0) ? v2.y : 1.0f), v1.z/((v2.z!=0) ? v2.z : 1.0f));
}

}
