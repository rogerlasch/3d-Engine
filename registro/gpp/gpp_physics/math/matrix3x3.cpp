

#include<sstream>
#include"vector3d.h"
#include"matrix3x3.h"

using namespace std;

namespace gpp
{
      // Constructor
       matrix3x3::matrix3x3(void)
      {
             e11 = 0;
             e12 = 0;
             e13 = 0;
             e21 = 0;
             e22 = 0;
             e23 = 0;
             e31 = 0;
             e32 = 0;
             e33 = 0;
      }

      // Constructor
       matrix3x3::matrix3x3(float r1c1, float r1c2, float r1c3,                                                       float r2c1, float r2c2, float r2c3,                                                       float r3c1, float r3c2, float r3c3 )
      {
             e11 = r1c1;
             e12 = r1c2;
             e13 = r1c3;
             e21 = r2c1;
             e22 = r2c2;
             e23 = r2c3;
             e31 = r3c1;
             e32 = r3c2;
             e33 = r3c3;
      }

matrix3x3::matrix3x3(const matrix3x3& mt)
{
*this=mt;
}

matrix3x3& matrix3x3::operator=(const matrix3x3& mt)
{
this->e11=mt.e11;
this->e12=mt.e12;
this->e13=mt.e13;
this->e21=mt.e21;
this->e22=mt.e22;
this->e23=mt.e23;
this->e31=mt.e31;
this->e32=mt.e32;
this->e33=mt.e33;
return *this;
}

 matrix3x3& matrix3x3::operator+=(const matrix3x3& m)
{
       e11 += m.e11;
             e12 += m.e12;
             e13 += m.e13;
             e21 += m.e21;
             e22 += m.e22;
             e23 += m.e23;
             e31 += m.e31;
             e32 += m.e32;
             e33 += m.e33;
             return *this;
      }

       matrix3x3& matrix3x3::operator-=(const matrix3x3& m)
      {
             e11 -= m.e11;
             e12 -= m.e12;
             e13 -= m.e13;
             e21 -= m.e21;
             e22 -= m.e22;
             e23 -= m.e23;
             e31 -= m.e31;
             e32 -= m.e32;
             e33 -= m.e33;
             return *this;
      }

       matrix3x3& matrix3x3::operator*=(float s)
      {
             e11 *= s;
             e12 *= s;
             e13 *= s;
             e21 *= s;
             e22 *= s;
             e23 *= s;
             e31 *= s;
             e32 *= s;
             e33 *= s;
             return *this;
      }

       matrix3x3& matrix3x3::operator/=(float s)
      {
if(s==0.0f) s=1.0f;
             e11 /= s;
             e12 /= s;
             e13 /= s;
             e21 /= s;
             e22 /= s;
             e23 /= s;
             e31 /= s;
             e32 /= s;
             e33 /= s;
             return *this;
      }

void matrix3x3::clear()
{
             e11 = 0;
             e12 = 0;
             e13 = 0;
             e21 = 0;
             e22 = 0;
             e23 = 0;
             e31 = 0;
             e32 = 0;
             e33 = 0;
}

       float matrix3x3::det(void)
      {
             return e11*e22*e33 -
                            e11*e32*e23 +
                            e21*e32*e13 -
                            e21*e12*e33 +
                            e31*e12*e23 -
                            e31*e22*e13;
      }

matrix3x3 matrix3x3::Transpose(void)
      {
             return matrix3x3(e11,e21,e31,e12,e22,e32,e13,e23,e33);
      }

matrix3x3 matrix3x3::Inverse(void)
      {
             float d = e11*e22*e33 -
                                  e11*e32*e23 +
                                  e21*e32*e13 -
                                  e21*e12*e33 +
                                  e31*e12*e23 -
                                  e31*e22*e13;
if (d == 0) d = 1;
       return  matrix3x3(             (e22*e33-e23*e32)/d,
                                   -(e12*e33-e13*e32)/d,
                                     (e12*e23-e13*e22)/d,
                                     -(e21*e33-e23*e31)/d,
                                     (e11*e33-e13*e31)/d,
                                     -(e11*e23-e13*e21)/d,
                                     (e21*e32-e22*e31)/d,
                                     -(e11*e32-e12*e31)/d,
                                     (e11*e22-e12*e21)/d );
}

void matrix3x3::Identity()
{
this->clear();
e11=1.0f;
e22=1.0f;
e33=1.0f;
}

void matrix3x3::setDiagonal(const vector3d& v)
{
e11=v.x;
e22=v.y;
e33=v.z;
}

vector3d matrix3x3::getDiagonal()const
{
return vector3d(e11, e22, e33);
}

string matrix3x3::toString()const
{
stringstream ss;
ss<<fixed;
ss<<e11<<","<<e12<<","<<e13<<endl;
ss<<e21<<","<<e22<<","<<e23<<endl;
ss<<e31<<","<<e32<<","<<e33<<endl;
return ss.str();
}

//Overloads

ostream& operator<<(ostream& os, const matrix3x3& mt)
{
os<<mt.toString();
return os;
}

//Overloads...
       matrix3x3 operator+(const matrix3x3& m1, const matrix3x3& m2)
      {
             return matrix3x3( m1.e11+m2.e11,
                                                   m1.e12+m2.e12,
                                                   m1.e13+m2.e13,
                                                   m1.e21+m2.e21,
                                                   m1.e22+m2.e22,
                                                   m1.e23+m2.e23,
                                                   m1.e31+m2.e31,
                                                   m1.e32+m2.e32,
                                                   m1.e33+m2.e33);
      }

       matrix3x3 operator/(float s, const matrix3x3& m)
      {
             return matrix3x3( m.e11/s,
                                                   m.e12/s,
                                                   m.e13/s,
                                                   m.e21/s,
                                                   m.e22/s,
                                                   m.e23/s,
                                                   m.e31/s,
                                                   m.e32/s,
                                                   m.e33/s);
      }

       matrix3x3 operator-(const matrix3x3& m1, const matrix3x3& m2)
      {
             return matrix3x3( m1.e11-m2.e11,
                                                   m1.e12-m2.e12,
                                                   m1.e13-m2.e13,
                                                   m1.e21-m2.e21,
                                                   m1.e22-m2.e22,
                                                   m1.e23-m2.e23,
                                                   m1.e31-m2.e31,
                                                   m1.e32-m2.e32,
                                                   m1.e33-m2.e33);
      }

       matrix3x3 operator/(const matrix3x3& m, float s)
      {
             return matrix3x3( m.e11/s,
                                                   m.e12/s,
                                                   m.e13/s,
                                                   m.e21/s,
                                                   m.e22/s,
                                                   m.e23/s,
                                                   m.e31/s,
                                                   m.e32/s,
                                                   m.e33/s);
      }

       matrix3x3 operator*(const matrix3x3& m1, const matrix3x3& m2)
      {
             return matrix3x3(m1.e11*m2.e11 + m1.e12*m2.e21 + m1.e13*m2.e31,
                                       m1.e11*m2.e12 + m1.e12*m2.e22 + m1.e13*m2.e32,
                                       m1.e11*m2.e13 + m1.e12*m2.e23 + m1.e13*m2.e33,
                                       m1.e21*m2.e11 + m1.e22*m2.e21 + m1.e23*m2.e31,
                                       m1.e21*m2.e12 + m1.e22*m2.e22 + m1.e23*m2.e32,
                                       m1.e21*m2.e13 + m1.e22*m2.e23 + m1.e23*m2.e33,
                                       m1.e31*m2.e11 + m1.e32*m2.e21 + m1.e33*m2.e31,
                                       m1.e31*m2.e12 + m1.e32*m2.e22 + m1.e33*m2.e32,
                                       m1.e31*m2.e13 + m1.e32*m2.e23 + m1.e33*m2.e33 );
      }

       matrix3x3 operator*(const matrix3x3& m, float s)
      {
             return matrix3x3( m.e11*s,
                                                   m.e12*s,
                                                   m.e13*s,
                                                   m.e21*s,
                                                   m.e22*s,
                                                   m.e23*s,
                                                   m.e31*s,
                                                   m.e32*s,
                                                   m.e33*s);
      }

       matrix3x3 operator*(float s, const matrix3x3& m)
      {
             return matrix3x3( m.e11*s,
                                                   m.e12*s,
                                                   m.e13*s,
                                                   m.e21*s,
                                                   m.e22*s,
                                                   m.e23*s,
                                                   m.e31*s,
                                                   m.e32*s,
                                                   m.e33*s);
      }

       vector3d operator*(const matrix3x3& m, const vector3d& u)
      {
             return vector3d( m.e11*u.x + m.e12*u.y + m.e13*u.z,
                                           m.e21*u.x + m.e22*u.y + m.e23*u.z,
                                           m.e31*u.x + m.e32*u.y + m.e33*u.z);
}

 vector3d operator*(const vector3d& u, const matrix3x3& m)
{
       return vector3d( u.x*m.e11 + u.y*m.e21 + u.z*m.e31,
                                     u.x*m.e12 + u.y*m.e22 + u.z*m.e32,
                                     u.x*m.e13 + u.y*m.e23 + u.z*m.e33);
}
}
