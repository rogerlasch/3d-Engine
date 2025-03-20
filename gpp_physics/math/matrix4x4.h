
#ifndef MATRIX4X4_H
#define MATRIX4X4_H

#include <array>
#include<initializer_list>
#include <iostream>

namespace gpp {

    class matrix4x4 {
    private:
        std::array<decimal, 16> arr;

    public:
        matrix4x4();
        matrix4x4(const matrix4x4& mt);
        matrix4x4(const std::array<std::array<decimal, 4>, 4>& mt);
matrix4x4(const std::initializer_list<std::initializer_list<decimal>>& mt);
        matrix4x4& operator=(const matrix4x4& mt);
        matrix4x4& operator=(const std::array<std::array<decimal, 4>, 4>& mt);
matrix4x4& operator=(const std::initializer_list<std::initializer_list<decimal>>& mt);

//Overload operators...

//scalar...

matrix4x4& operator+=(decimal s);
matrix4x4& operator-=(decimal s);
matrix4x4& operator*=(decimal s);
matrix4x4& operator/=(decimal s);

matrix4x4& operator+=(const matrix4x4& mt);
matrix4x4& operator-=(const matrix4x4& mt);

//Methods...

        void zero();
        void identity();
        void setDiagonal(const std::array<decimal, 4>& d);
        std::array<decimal, 4> getDiagonal() const;

matrix4x4& negate();
matrix4x4& inverse();
matrix4x4& transpose();
decimal determinant()const;

static matrix4x4 negate(const matrix4x4& mt);
static matrix4x4 inverse(const matrix4x4& mt);
static matrix4x4 transpose(const matrix4x4& mt);
static decimal determinant(const matrix4x4& mt);
static matrix4x4 getTranslation(const vector3d& translation);
static matrix4x4 getScale(const vector3d& scale);
static matrix4x4 getRotation(const vector3d& angles);
static matrix4x4 getSRT(const vector3d& scale, const vector3d& angles, const vector3d& translation);
static matrix4x4 lerp(const matrix4x4& a, const matrix4x4& b, decimal t);
friend     std::ostream& operator<<(std::ostream& os, const matrix4x4& mt);
friend matrix4x4 operator*(const matrix4x4& mt1, const matrix4x4& mt2);
friend vector3d operator*(const matrix4x4& m, const vector3d& v);
friend vector3d operator*(const vector3d& v, const matrix4x4& m);

friend bool matrix4x4_isEqual(const matrix4x4& m1, const matrix4x4& m2, decimal tol);
    };

    std::ostream& operator<<(std::ostream& os, const matrix4x4& mt);
    std::ostream& operator<<(std::ostream& os, const std::array<decimal, 4>& arr);

//overloads...

matrix4x4 operator+(const matrix4x4& mt, decimal s);
matrix4x4 operator+(decimal s, const matrix4x4& mt);
matrix4x4 operator-(const matrix4x4& mt, decimal s);
matrix4x4 operator-(decimal s, const matrix4x4& mt);
matrix4x4 operator*(const matrix4x4& mt, decimal s);
matrix4x4 operator*(decimal s, const matrix4x4& mt);
matrix4x4 operator/(const matrix4x4& mt, decimal s);
matrix4x4 operator/(decimal s, const matrix4x4& mt);

matrix4x4 operator+(const matrix4x4& mt1, const matrix4x4& mt2);
matrix4x4 operator-(const matrix4x4& mt1, const matrix4x4& mt2);
matrix4x4 operator*(const matrix4x4& mt1, const matrix4x4& mt2);
vector3d operator*(const matrix4x4& m, const vector3d& v);
vector3d operator*(const vector3d& v, const matrix4x4& m);

bool operator==(const matrix4x4& m1, const matrix4x4& m2);
bool matrix4x4_isEqual(const matrix4x4& m1, const matrix4x4& m2, decimal tol);

}
#endif // MAdecimalRIX4X4_H
