
#ifndef MATRIX3X3_H
#define MATRIX3X3_H

#include<vector>

namespace gpp
{
class matrix3x3
{
private:
std::vector<std::vector<float>> mt;
public:
matrix3x3();
matrix3x3(const std::vector<std::vector<float>>& mt);
matrix3x3(const matrix3x3& mt);
matrix3x3& operator=(const std::vector<std::vector<float>>& mt);
matrix3x3& operator=(const matrix3x3& mt);
matrix3x3& operator+=(float s);
matrix3x3& operator-=(float s);
matrix3x3& operator*=(float s);
matrix3x3& operator/=(float s);
float& operator()(unsigned int i, unsigned int i1);
float operator()(unsigned int i, unsigned int i1)const;
float determinant();
matrix3x3 transpose();
matrix3x3 inverse();
void fill(float n);
void setIdentity();
friend inline void checkMatrix(matrix3x3* m);
};

matrix3x3 operator+(const matrix3x3& m, float s);
matrix3x3 operator+(float s, const matrix3x3& m);
matrix3x3 operator-(const matrix3x3& m, float s);
matrix3x3 operator-(float s, const matrix3x3& m);
matrix3x3 operator*(const matrix3x3& m, float s);
matrix3x3 operator*(float s, const matrix3x3& m);
matrix3x3 operator/(const matrix3x3& m, float s);
matrix3x3 operator/(float s, const matrix3x3& m);
//Multiplication...
matrix3x3 operator*(const matrix3x3& m1, const matrix3x3& m2);
vector3d operator*(const matrix3x3& m, const vector3d& v);
vector3d operator*(const vector3d& v, const matrix3x3& m);
}
#endif
