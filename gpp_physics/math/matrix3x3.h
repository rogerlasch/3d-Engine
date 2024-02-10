
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
             float e11, e12, e13, e21, e22, e23, e31, e32, e33;
matrix3x3(void);
matrix3x3(float r1c1, float r1c2, float r1c3,                 float r2c1, float r2c2, float r2c3,                float r3c1, float r3c2, float r3c3 );
matrix3x3(const matrix3x3& mt);
matrix3x3& operator=(const matrix3x3& mt);
             matrix3x3& operator+=(const matrix3x3& m);
             matrix3x3& operator-=(const matrix3x3& m);
             matrix3x3& operator*=(float s);
             matrix3x3& operator/=(float s);
void clear();
float      det(void);
matrix3x3  transpose(void);
matrix3x3  inverse(void);
void identity();
void setDiagonal(const vector3d& v);
vector3d getDiagonal()const;
std::string toString()const;
      };

std::ostream& operator<<(std::ostream& os, const matrix3x3& mt);
       matrix3x3 operator+(const matrix3x3& m1, const matrix3x3& m2);
       matrix3x3 operator-(const matrix3x3& m1, const matrix3x3& m2);
       matrix3x3 operator/(float s, const matrix3x3& m);
       matrix3x3 operator/(const matrix3x3& m, float s);
       matrix3x3 operator*(const matrix3x3& m1, const matrix3x3& m2);
       matrix3x3 operator*(const matrix3x3& m, float s);
       matrix3x3 operator*(float s, const matrix3x3& m);
       vector3d operator*(const matrix3x3& m, const vector3d& u);
 vector3d operator*(const vector3d& u, const matrix3x3& m);
}
#endif
