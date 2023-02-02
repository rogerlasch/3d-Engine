


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
//is the same formula as the distance
return sqrt((this->x*this->x)+(this->y*this->y)+(this->z*this->z));
}

//Normaliza o vetor...
void vector3d::normalize()
{
float len=this->length();
if(len<=GPP_VECTOR_TOLERANCE) len=1.0f;
this->x/=len;
this->y/=len;
this->z/=len;
if(fabs(this->x)<GPP_VECTOR_TOLERANCE) this->x=0.0f;
if(fabs(this->y)<GPP_VECTOR_TOLERANCE) this->y=0.0f;
if(fabs(this->z)<GPP_VECTOR_TOLERANCE) this->z=0.0f;
}

//Inverte o vetor...
void vector3d::reverse()
{
this->x=-this->x;
this->y=-this->y;
this->z=-this->z;
}

//Inverte o vetor retornando um novo vetor.
vector3d vector3d::reverse(const vector3d& v)
{
return vector3d(-v.x, -v.y, -v.z);
}

//recupera uma das 3 coordenadas possíveis...
float vector3d::get(int axis)const
{
switch(axis)
{
case 0:
return this->x;
case 1:
return this->y;
case 2:
return this-> z;
}
return -1;
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

//Normaliza o vetor, versão estática.
//Útil quando precisamos manter o original intácto.
vector3d vector3d::normalize(const vector3d& v)
{
float len=v.length();
if(len<=GPP_VECTOR_TOLERANCE) len=1.0f;
vector3d v2(v.x/len, v.y/len, v.z/len);
if(fabs(v2.x)<GPP_VECTOR_TOLERANCE) v2.x=0.0f;
if(fabs(v2.y)<GPP_VECTOR_TOLERANCE) v2.y=0.0f;
if(fabs(v2.z)<GPP_VECTOR_TOLERANCE) v2.z=0.0f;
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
float vector3d::tripleProduct(const vector3d& v1, const vector3d& v2, const vector3d& v3)
{
             return float( (v1.x * (v2.y*v3.z - v2.z*v3.y)) + (v1.y * (-v2.x*v3.z + v2.z*v3.x)) + (v1.z * (v2.x*v3.y - v2.y*v3.x)));
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
if(v==-0)
{
return 0.0f;
}
return v;
}

//Functions

ostream& operator<<(ostream& os, const vector3d& dv)
{
os.precision(2);
os<<fixed;
os<<dv.x<<":"<<dv.y<<":"<<dv.z<<endl;
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
