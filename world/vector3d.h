

#ifndef VECTOR3D_H
#define VECTOR3D_H

#include<iostream>
namespace gpp
{

enum gpp_directions
{
gpp_invalid_dir=-1,
gpp_north=0,
gpp_northeast=45,
gpp_east=90,
gpp_southeast=135,
gpp_south=180,
gpp_southwest=225,
gpp_west=270,
gpp_northwest=315
};

#define GPP_PI 3.1415926

class vector3d
{
public:
float x;
float y;
float z;
vector3d(float x=0.0f, float y=0.0f, float z=0.0f);
vector3d(const vector3d& dv);
vector3d& operator=(const vector3d& dv);
vector3d& operator=(const std::initializer_list<float>& points);
bool operator==(const vector3d& dv)const;
bool operator==(const std::initializer_list<float>& dv)const;
vector3d operator-(const vector3d& dv)const;
vector3d& operator-=(const vector3d& dv);
vector3d operator-(float escalar);
vector3d& operator-=(float escalar);
vector3d operator+(const vector3d& dv);
vector3d& operator+=(const vector3d& dv);
vector3d operator+(float escalar);
vector3d& operator+=(float escalar);
vector3d operator*(const vector3d& dv);
vector3d& operator*=(const vector3d& dv);
vector3d operator*(float escalar);
vector3d& operator*=(float escalar);
vector3d operator/(const vector3d& v);
vector3d& operator/=(const vector3d& v);
vector3d operator/(float escalar);
vector3d& operator/=(float escalar);
float length()const;
float scalar_product(const vector3d& dv);
float angle_from(const vector3d& dv);
vector3d vector_product();
float get_x()const;
float get_y()const;
float get_z()const;
void get_xyz(float * x, float* y, float* z);
void set_x(float x);
void set_y(float y);
void set_z(float z);
void set_xyz(float x, float y, float z);
float get_distance(const vector3d& dv)const;
float get_squared_distance(const vector3d& v)const;
};

std::ostream& operator<<(std::ostream& os, const vector3d& dv);
}
#endif
