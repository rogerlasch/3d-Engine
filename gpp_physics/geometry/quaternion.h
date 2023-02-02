


#ifndef QUATERNION_H
#define QUATERNION_H

namespace gpp
{

class quaternion
{
public:
float n;
vector3d v;
quaternion(float n=0.0f, float x=0.0f, float y=0.0f, float z=0.0f);
quaternion(const quaternion& q);
quaternion& operator=(const quaternion& q);
quaternion& operator+=(float s);
quaternion& operator-=(float s);
quaternion& operator*=(float s);
quaternion& operator/=(float s);
quaternion& operator+=(const quaternion& q);
quaternion& operator-=(const quaternion& q);
quaternion operator~()const;
void setScalar(float n);
float getScalar()const;
void setVector(const vector3d& v);
vector3d getVector()const;
float length();
quaternion conjugate()const;
float getAngle()const;
vector3d getAxis()const;

};

quaternion operator+(const quaternion& q, float s);
quaternion operator+(float s, const quaternion& q);
quaternion operator-(const quaternion& q, float s);
quaternion operator-(float s, const quaternion& q);
quaternion operator*(const quaternion& q, float s);
quaternion operator*(float s, const quaternion& q);
quaternion operator/(const quaternion& q, float s);
quaternion operator/(float s, const quaternion& q);

quaternion operator+(const quaternion& q1, const quaternion& q2);
quaternion operator-(const quaternion& q1, const quaternion& q2);
quaternion operator*(const quaternion& q1, const quaternion& q2);
quaternion operator/(const quaternion& q1, const quaternion& q2);

quaternion operator*(const quaternion& q, const vector3d& v);
quaternion operator*(const vector3d& v, quaternion& q);
quaternion quaternion_rotate(const quaternion& q1, const quaternion& q2);
vector3d quaternion_vector_rotate(const quaternion& q, const vector3d& v);
quaternion quaternion_from_euler_angles(float x, float y, float z);
vector3d quaternion_extract_euler_angles(const quaternion& q);
}
#endif
