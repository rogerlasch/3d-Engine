

/**
*Classe que representa um vetor 3d com os componentes x, y e z.
*Também tem sobrecargas para muitas operações.
**/
#ifndef VECTOR3D_H
#define VECTOR3D_H

#include<libmorton/morton.h>
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

class vector3d
{
public:
static const vector3d ORIGIN;
decimal x;
decimal y;
decimal z;

vector3d(decimal x=0.0f, decimal y=0.0f, decimal z=0.0f);
vector3d(const vector3d& dv);
vector3d& operator=(const vector3d& dv);
vector3d& operator=(const decimal  points[3]);
decimal& operator[](unsigned int index);
decimal operator[](unsigned int index)const;
bool operator==(const vector3d& dv)const;
bool operator==(const decimal dv[3])const;
vector3d operator-();
vector3d& operator+=(decimal escalar);
vector3d& operator-=(decimal escalar);
vector3d& operator*=(decimal escalar);
vector3d& operator/=(decimal escalar);
vector3d& operator-=(const vector3d& dv);
vector3d& operator+=(const vector3d& dv);
vector3d& operator*=(const vector3d& dv);
vector3d& operator/=(const vector3d& v);
void zero();
decimal length()const;
decimal lengthSquared()const;
vector3d& normalize();
vector3d& inverse();

decimal dot(const vector3d& v)const;
vector3d cross(const vector3d& v)const;

//Métodos estáticos
//Usado para operações com mais de um vetor.
//Operações como produto escalar, produto cruzado e triplo produto escalar.

static vector3d normalize(const vector3d& v);
static vector3d inverse(const vector3d& v);
static decimal dot(const vector3d& v1, const vector3d& v2);
static vector3d cross(const vector3d& v1, const vector3d& v2);
static decimal triple(const vector3d& v1, const vector3d& v2, const vector3d& v3);
std::string toString()const;
};

//Sobrecarga de operadores
//As sobrecargas aqui são de multiplicação, divisão, soma e subtração.
//Também o operator<< é sobrecarregado para uso fácil com ostreams...
std::ostream& operator<<(std::ostream& os, const vector3d& dv);
vector3d operator+(const vector3d& v, decimal s);
vector3d operator+(decimal s, const vector3d& v);
vector3d operator-(const vector3d& v, decimal s);
vector3d operator-(decimal s, const vector3d& v);
vector3d operator*(const vector3d& v, decimal s);
vector3d operator*(decimal s, const vector3d& v);
vector3d operator/(const vector3d& v, decimal s);
vector3d operator/(decimal s, const vector3d& v);

vector3d operator+(const vector3d& v1, const vector3d& v2);
vector3d operator-(const vector3d& v1, const vector3d& v2);
decimal operator*(const vector3d& v1, const vector3d& v2);
vector3d multiVec(const vector3d& v1, const vector3d& v2);
vector3d operator/(const vector3d& v1, const vector3d& v2);
vector3d operator^(const vector3d& v1, const vector3d& v2);
bool vector3dIsEqual(const vector3d& v1, const vector3d& v2, decimal tol=0.1f);
}

namespace std{
    template <>
    struct hash<gpp::vector3d> {
        std::size_t operator()(const gpp::vector3d& v) const noexcept {
            return libmorton::morton3D_32_encode(static_cast<int>(v.x), static_cast<int>(v.y), static_cast<int>(v.z));
        }
    };
}
#endif
