
/**
*Classe para uma matriz 3x3 útil em algumas situações.
*Esta classe foi retirada do livro physics for game developers.
*Alguns metodos foram adicionados a ela.
**/
#ifndef MATRIX3X3_H
#define MATRIX3X3_H

#include<iostream>

namespace gpp
{
      class matrix3x3 {
      public:
std::array<decimal, 9> arr;
        matrix3x3();
        matrix3x3(const matrix3x3& mt);
        matrix3x3(const std::array<std::array<decimal, 3>, 3>& mt);
matrix3x3(const std::initializer_list<std::initializer_list<decimal>>& mt);
        matrix3x3& operator=(const matrix3x3& mt);
        matrix3x3& operator=(const std::array<std::array<decimal, 3>, 3>& mt);
matrix3x3& operator=(const std::initializer_list<std::initializer_list<decimal>>& mt);

//Overload operators...

//scalar...

matrix3x3& operator+=(decimal s);
matrix3x3& operator-=(decimal s);
matrix3x3& operator*=(decimal s);
matrix3x3& operator/=(decimal s);

matrix3x3& operator+=(const matrix3x3& mt);
matrix3x3& operator-=(const matrix3x3& mt);

//Methods...

vector3d getRow(uint32 index)const;
matrix3x3& zero();
        matrix3x3& identity();
matrix3x3& setDiagonal(const std::array<decimal, 3>& d);
        std::array<decimal, 3> getDiagonal() const;

matrix3x3& negate();
matrix3x3& inverse();
matrix3x3& transpose();
decimal determinant()const;

static matrix3x3 negate(const matrix3x3& mt);
static matrix3x3 inverse(const matrix3x3& mt);
static matrix3x3 transpose(const matrix3x3& mt);
static decimal determinant(const matrix3x3& mt);
static matrix3x3 getTranslation(const vector3d& translation);
static matrix3x3 getScale(const vector3d& scale);
static matrix3x3 getRotation(const vector3d& angles);
static matrix3x3 getSRT(const vector3d& scale, const vector3d& angles, const vector3d& translation);
static matrix3x3 lerp(const matrix3x3& a, const matrix3x3& b, decimal t);
friend     std::ostream& operator<<(std::ostream& os, const matrix3x3& mt);
friend matrix3x3 operator*(const matrix3x3& mt1, const matrix3x3& mt2);
friend vector3d operator*(const matrix3x3* m, const vector3d& v);
friend vector3d operator*(const vector3d& v, const matrix3x3& m);
friend bool matrix3x3_isEqual(const matrix3x3& m1, const matrix3x3& m2, decimal tol);

      };

    std::ostream& operator<<(std::ostream& os, const matrix3x3& mt);
    std::ostream& operator<<(std::ostream& os, const std::array<decimal, 3>& arr);

//overloads...

matrix3x3 operator+(const matrix3x3& mt, decimal s);
matrix3x3 operator+(decimal s, const matrix3x3& mt);
matrix3x3 operator-(const matrix3x3& mt, decimal s);
matrix3x3 operator-(decimal s, const matrix3x3& mt);
matrix3x3 operator*(const matrix3x3& mt, decimal s);
matrix3x3 operator*(decimal s, const matrix3x3& mt);
matrix3x3 operator/(const matrix3x3& mt, decimal s);
matrix3x3 operator/(decimal s, const matrix3x3& mt);

matrix3x3 operator+(const matrix3x3& mt1, const matrix3x3& mt2);
matrix3x3 operator-(const matrix3x3& mt1, const matrix3x3& mt2);
matrix3x3 operator*(const matrix3x3& mt1, const matrix3x3& mt2);

vector3d operator*(const matrix3x3& m, const vector3d& v);
vector3d operator*(const vector3d& v, const matrix3x3& m);

bool operator==(const matrix3x3& m1, const matrix3x3& m2);
bool matrix3x3_isEqual(const matrix3x3& m1, const matrix3x3& m2, decimal tol);
}
#endif
