

#include<sstream>
#include"vector3d.h"
#include"matrix3x3.h"

using namespace std;

namespace gpp
{
void checkMatrix(matrix3x3* m)
{
if(m!=NULL)
{
if(m->mt.size()!=3) m->mt.resize(3);
for(int i=0; i<3; i++)
{
if(m->mt[i].size()!=3) m->mt[i].resize(3, 0);
}
}
}

matrix3x3::matrix3x3()
{
mt.resize(3);
mt[0].resize(3, 0);
mt[1].resize(3, 0);
mt[2].resize(3, 0);
}

matrix3x3::matrix3x3(const vector<vector<float>>& mt)
{
*this=mt;
}

matrix3x3::matrix3x3(const matrix3x3& mt)
{
*this=mt;
}

matrix3x3& matrix3x3::operator=(const vector<vector<float>>& mt)
{
this->mt=mt;
checkMatrix(this);
return *this;
}

matrix3x3& matrix3x3::operator=(const matrix3x3& mt)
{
this->mt=mt.mt;
checkMatrix(this);
return *this;
}

matrix3x3& matrix3x3::operator+=(float s)
{
for(int i=0; i<3; i++)
{
for(int i1=0; i1<3; i1++)
{
mt[i][i1]+=s;
}
}
return *this;
}

matrix3x3& matrix3x3::operator-=(float s)
{
for(int i=0; i<3; i++)
{
for(int i1=0; i1<3; i1++)
{
mt[i][i1]-=s;
}
}
return *this;
}

matrix3x3& matrix3x3::operator*=(float s)
{
for(int i=0; i<3; i++)
{
for(int i1=0; i1<3; i1++)
{
mt[i][i1]*=s;
}
}
return *this;
}

matrix3x3& matrix3x3::operator/=(float s)
{
for(int i=0; i<3; i++)
{
for(int i1=0; i1<3; i1++)
{
mt[i][i1]/=s;
}
}
return *this;
}

float& matrix3x3::operator()(unsigned int i, unsigned int i1)
{
if((i>2)||(i1>2))
{
throw runtime_error("Matrix3x3 out of bounds.");
}
return mt[i][i1];
}

float matrix3x3::operator()(unsigned int i, unsigned int i1)const
{
if((i>2)||(i1>2))
{
throw runtime_error("Matrix3x3 out of bounds.");
}
return mt[i][i1];
}

float matrix3x3::determinant()
{
             return mt[0][0]*mt[1][1]*mt[2][2] -
mt[0][0]*mt[2][1]*mt[1][2] +
mt[1][0]*mt[2][1]*mt[0][2] -
mt[1][0]*mt[0][1]*mt[2][2] +
mt[2][0]*mt[0][1]*mt[1][2] -
mt[2][0]*mt[1][1]*mt[0][2];
}

matrix3x3 matrix3x3::transpose()
{
matrix3x3 m({{mt[0][0], mt[1][0], mt[2][0]}, {mt[0][1], mt[1][1], mt[2][1]}, {mt[0][2], mt[1][2], mt[2][2]}});
return m;
}
matrix3x3 matrix3x3::inverse()
{
float d=this->determinant();
if(d==0.0f) d=1.0f;
       return matrix3x3({{(mt[1][1]*mt[2][2]-mt[1][2]*mt[2][1])/d, -(mt[0][1]*mt[2][2]-mt[0][2]*mt[2][1])/d, (mt[0][1]*mt[1][2]-mt[0][2]*mt[1][1])/d},
{-(mt[1][0]*mt[2][2]-mt[1][2]*mt[2][0])/d, (mt[0][0]*mt[2][2]-mt[0][2]*mt[2][0])/d, -(mt[0][0]*mt[1][2]-mt[0][2]*mt[1][0])/d},
{                                     (mt[1][0]*mt[2][1]-mt[1][1]*mt[2][0])/d,                                      -(mt[0][0]*mt[2][1]-mt[0][1]*mt[2][0])/d, (mt[0][0]*mt[1][1]-mt[0][1]*mt[1][0])/d}});
}

void matrix3x3::fill(float n)
{
for(int i=0; i<3; i++)
{
for(int i1=0; i1<3; i1++)
{
mt[i][i1]=n;
}
}
}

void matrix3x3::setIdentity()
{
fill(0.0f);
mt[0][0]=1.0f;
mt[1][1]=1.0f;
mt[2][2]=1.0f;
}

void matrix3x3::setDiagonal(const vector3d& v)
{
mt[0][0]=v.x;
mt[1][1]=v.y;
mt[2][2]=v.z;
}

vector3d matrix3x3::getDiagonal()const
{
return vector3d(mt[0][0], mt[1][1], mt[2][2]);
}

string matrix3x3::toString()const
{
stringstream ss;
ss<<fixed;
ss.precision(2);
for(auto& it : mt)
{
for(auto& it2 : it)
{
ss<<it2<<" ";
}
ss<<endl;
}
return ss.str();
}

//Overloads...

ostream& operator<<(ostream& os, const matrix3x3& mt)
{
return os<<mt.toString();
}

matrix3x3 operator+(const matrix3x3& m, float s)
{
return matrix3x3({{m(0,0)+s, m(0,1)+s, m(0,2)+s},
{m(1,0)+s, m(1,1)+s, m(1,2)+s},
{m(2,0)+s, m(2,1)+s, m(2,2)+s}});
}

matrix3x3 operator+(float s, const matrix3x3& m)
{
return matrix3x3({{m(0,0)+s, m(0,1)+s, m(0,2)+s},
{m(1,0)+s, m(1,1)+s, m(1,2)+s},
{m(2,0)+s, m(2,1)+s, m(2,2)+s}});
}

matrix3x3 operator-(const matrix3x3& m, float s)
{
return matrix3x3({{m(0,0)-s, m(0,1)-s, m(0,2)-s},
{m(1,0)-s, m(1,1)-s, m(1,2)-s},
{m(2,0)-s, m(2,1)-s, m(2,2)-s}});
}

matrix3x3 operator-(float s, const matrix3x3& m)
{
return matrix3x3({{m(0,0)-s, m(0,1)-s, m(0,2)-s},
{m(1,0)-s, m(1,1)-s, m(1,2)-s},
{m(2,0)-s, m(2,1)-s, m(2,2)-s}});
}

matrix3x3 operator*(const matrix3x3& m, float s)
{
return matrix3x3({{m(0,0)*s, m(0,1)*s, m(0,2)*s},
{m(1,0)*s, m(1,1)*s, m(1,2)*s},
{m(2,0)*s, m(2,1)*s, m(2,2)*s}});
}

matrix3x3 operator*(float s, const matrix3x3& m)
{
return matrix3x3({{m(0,0)*s, m(0,1)*s, m(0,2)*s},
{m(1,0)*s, m(1,1)*s, m(1,2)*s},
{m(2,0)*s, m(2,1)*s, m(2,2)*s}});
}

matrix3x3 operator/(const matrix3x3& m, float s)
{
return matrix3x3({{m(0,0)/s, m(0,1)/s, m(0,2)/s},
{m(1,0)/s, m(1,1)/s, m(1,2)/s},
{m(2,0)/s, m(2,1)/s, m(2,2)/s}});
}

matrix3x3 operator/(float s, const matrix3x3& m)
{
return matrix3x3({{m(0,0)/s, m(0,1)/s, m(0,2)/s},
{m(1,0)/s, m(1,1)/s, m(1,2)/s},
{m(2,0)/s, m(2,1)/s, m(2,2)/s}});
}

matrix3x3 operator*(const matrix3x3& m1, const matrix3x3& m2)
{
             return matrix3x3({{m1(0,0)*m2(0,0) + m1(0,1)*m2(1,0) + m1(0,2)*m2(2,0), m1(0,0)*m2(0,1) + m1(0,1)*m2(1,1) + m1(0,2)*m2(2,1), m1(0,0)*m2(0,2) + m1(0,1)*m2(1,2) + m1(0,2)*m2(2,2)},
{m1(1,0)*m2(0,0) + m1(1,1)*m2(1,0) + m1(1,2)*m2(2,0), m1(1,0)*m2(0,1) + m1(1,1)*m2(1,1) + m1(1,2)*m2(2,1), m1(1,0)*m2(0,2) + m1(1,1)*m2(1,2) + m1(1,2)*m2(2,2)},
{m1(2,0)*m2(0,0) + m1(2,1)*m2(1,0) + m1(2,2)*m2(2,0), m1(2,0)*m2(0,1) + m1(2,1)*m2(1,1) + m1(2,2)*m2(2,1), m1(2,0)*m2(0,2) + m1(2,1)*m2(1,2) + m1(2,2)*m2(2,2)}});
}

vector3d operator*(const matrix3x3& m, const vector3d& v)
{
return vector3d( v.x*m(0,0) + v.y*m(1,0) + v.z*m(2,0), v.x*m(0,1) + v.y*m(1,1) + v.z*m(2,1), v.x*m(0,2) + v.y*m(1,2) + v.z*m(2,2));
}

vector3d operator*(const vector3d& v, const matrix3x3& m)
{
return vector3d( v.x*m(0,0) + v.y*m(1,0) + v.z*m(2,0), v.x*m(0,1) + v.y*m(1,1) + v.z*m(2,1), v.x*m(0,2) + v.y*m(1,2) + v.z*m(2,2));
}
}
