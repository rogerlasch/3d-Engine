
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
             // elements eij: i -> row, j -> column
             decimal e11, e12, e13, e21, e22, e23, e31, e32, e33;
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

        void zero();
        void identity();
        void setDiagonal(const std::array<decimal, 3>& d);
        std::array<decimal, 3> getDiagonal() const;

matrix3x3& negate();
matrix3x3& inverse();
matrix3x3& transpose();
decimal determinant()const;

static matrix3x3 negate(const matrix3x3& mt);
static matrix3x3 inverse(const matrix3x3& mt);
static matrix3x3 transpose(const matrix3x3& mt);
static decimal determinant(const matrix3x3& mt);

friend     std::ostream& operator<<(std::ostream& os, const matrix3x3& mt);
friend matrix3x3 operator*(const matrix3x3& mt1, const matrix3x3& mt2);
friend vector3d operator*(const matrix3x3* m, const vector3d& v);
friend vector3d operator*(const vector3d& v, const matrix3x3& m);

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
}
#endif
