

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

//Define PI constant
#define GPP_PI 3.1415926
//Define tolerance value for floating point values...
#define GPP_VECTOR_TOLERANCE 0.0001f

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
vector3d& operator+=(float escalar);
vector3d& operator-=(float escalar);
vector3d& operator*=(float escalar);
vector3d& operator/=(float escalar);
vector3d& operator-=(const vector3d& dv);
vector3d& operator+=(const vector3d& dv);
vector3d& operator*=(const vector3d& dv);
vector3d& operator/=(const vector3d& v);
bool isNull()const;
//Calcula o comprimento, ou maguinitude do vetor...
float length()const;
//Normaliza o vetor...
void normalize();
//Inverte as propriedades do vetor...
void reverse();
float get(int axis)const;
float get_x()const;
float get_y()const;
float get_z()const;
void get_xyz(float * x, float* y, float* z);
void set_x(float x);
void set_y(float y);
void set_z(float z);
void set_xyz(float x, float y, float z);
//Métodos estáticos
//Usado para operações com mais de um vetor.
//Operações como produto escalar, produto cruzado e triplo produto escalar.
//Retorna um novo vetor normalizado, mantendo as propriedades originais intactas.
static vector3d normalize(const vector3d& v);
//Retorna o vetor inverso mantendo as propriedades originais.
static vector3d reverse(const vector3d& v);
//Calcula o produto escalar de 2 vetores...
static float dotProduct(const vector3d& v1, const vector3d& v2);
//Calcula o produto vetorial, ou o produto cruzado de 2 vetores...
static vector3d crozProduct(const vector3d& v1, const vector3d& v2);
//Calcula o produto triplo escalar de 3 vetores...
static float tripleProduct(const vector3d& v1, const vector3d& v2, const vector3d& v3);
//Calcula o ângulo entre 2 vetores...
static float angle_from(const vector3d& v1, const vector3d& v2);
//Calcula a distância entre 2 vetores.
static float get_distance(const vector3d& v1, const vector3d& v2);
//Retorna a distância ao quadrado entre 2 vetores.
static float get_squared_distance(const vector3d& v1, const vector3d& v2);
//Limpa um float convertendo nan e inf em 0.
static float floatClean(float v);
};

//Sobrecarga de operadores
//As sobrecargas aqui são de multiplicação, divisão, soma e subtração.
//Também o operator<< é sobrecarregado para uso fácil com ostreams...
std::ostream& operator<<(std::ostream& os, const vector3d& dv);
vector3d operator+(const vector3d& v, float s);
vector3d operator+(float s, const vector3d& v);
vector3d operator-(const vector3d& v, float s);
vector3d operator-(float s, const vector3d& v);
vector3d operator*(const vector3d& v, float s);
vector3d operator*(float s, const vector3d& v);
vector3d operator/(const vector3d& v, float s);
vector3d operator/(float s, const vector3d& v);

vector3d operator+(const vector3d& v1, const vector3d& v2);
vector3d operator-(const vector3d& v1, const vector3d& v2);
vector3d operator*(const vector3d& v1, const vector3d& v2);
vector3d operator/(const vector3d& v1, const vector3d& v2);
}
#endif
