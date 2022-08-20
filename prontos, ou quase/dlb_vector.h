

#ifndef DLB_VECTOR_H
#define DLB_VECTOR_H

namespace dlb
{

enum dlb_axis
{
ax_x=0,
ax_y,
ax_z
};

enum dlb_directions
{
dlb_invalid_dir=-1,
dlb_north=0,
dlb_northeast=45,
dlb_east=90,
dlb_southeast=135,
dlb_south=180,
dlb_southwest=225,
dlb_west=270,
dlb_northwest=315
};

#define dlb_pi 3.1415926
#define dlb_axis_all {ax_x, ax_y, ax_z}

class dlb_vector
{
public:
float x;
float y;
float z;
dlb_vector(float x=0.0f, float y=0.0f, float z=0.0f);
dlb_vector(const dlb_vector& dv);
dlb_vector& operator=(const dlb_vector& dv);
dlb_vector& operator=(const std::initializer_list<float>& points);
bool operator==(const dlb_vector& dv)const;
bool operator==(const std::initializer_list<float>& dv)const;
dlb_vector operator-(const dlb_vector& dv);
dlb_vector operator-=(const dlb_vector& dv);
dlb_vector operator+(const dlb_vector& dv);
dlb_vector operator+=(const dlb_vector& dv);
dlb_vector operator*(const dlb_vector& dv);
dlb_vector operator*=(const dlb_vector& dv);
dlb_vector operator*(float escalar);
dlb_vector operator*=(float escalar);
float length()const;
float scalar_product(const dlb_vector& dv);
float angle_from(const dlb_vector& dv);
dlb_vector vector_product();
float get_x()const;
float get_y()const;
float get_z()const;
void get_xyz(float * x, float* y, float* z);
void set_x(float x);
void set_y(float y);
void set_z(float z);
void set_xyz(float x, float y, float z);
float get_1d_distance(const dlb_vector& dv, int axis=ax_x)const;
float get_2d_distance(const dlb_vector& dv)const;
float get_3d_distance(const dlb_vector& dv)const;
dlb_vector get_distance(const dlb_vector& dv, const std::initializer_list<int>& axis=dlb_axis_all);
};

std::ostream& operator<<(std::ostream& os, const dlb_vector& dv);

}
#endif
