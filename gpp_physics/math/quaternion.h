

/**
*Classe de quat�rnion para a simula��o.
*Esta classe tamb�m foi retirada do livro Physics for game developers.
*V�rios m�todos adicionais foram adicionados.
*Esta classe serve para armazenar a orienta��o de um corpo no espa�o.
**/

#ifndef QUATERNION_H
#define QUATERNION_H

namespace gpp
{

/*
* Classe de quat�rnions fortemente baseada na classe quaternion do livro Physics for Game Developers.
**/
class quaternion
{
public:
decimal x;
decimal y;
decimal z;
decimal w;
quaternion(decimal x=0.0f, decimal y=0.0f, decimal z=0.0f, decimal w=0.0f);
quaternion(const vector3d& axis, decimal w);
quaternion(const quaternion& q);
quaternion& operator=(const quaternion& q);
bool operator==(const quaternion& q);
quaternion& operator+=(decimal s);
quaternion& operator-=(decimal s);
quaternion& operator*=(decimal s);
quaternion& operator/=(decimal s);
quaternion& operator+=(const quaternion& q);
quaternion& operator-=(const quaternion& q);
quaternion operator~()const;
void setX(decimal x);
decimal getX()const;
void setY(decimal y);
decimal getY()const;
void setZ(decimal z);
decimal getZ()const;
void setW(decimal w);
decimal getW()const;
//Calcula o comprimento do quat�rnion...
decimal length()const;
//M�todo de conjugar o quat�rnion...
quaternion conjugate()const;
//Normaliza o quaternion...
quaternion& normalize();
quaternion& setIdentity();
//Retorna o �ngulo que o quat�rnion est� representando...
decimal getAngle()const;
vector3d getAxis()const;
std::string toString()const;
matrix3x3 toMatrix3x3()const;
matrix4x4 toMatrix4x4() const;

    static quaternion slerp(const quaternion& q1, const quaternion& q2, decimal t);
    static quaternion lerp(const quaternion& q1, const quaternion& q2, decimal t);
static quaternion fromAxis(const vector3d& axis, decimal w);
static quaternion fromEuler(decimal x, decimal y, decimal z);
static quaternion normalize(const quaternion& q);
};

//Sobrecarga dos operadores do quat�rnion...

std::ostream& operator<<(std::ostream& os, const quaternion& q);
quaternion operator+(const quaternion& q, decimal s);
quaternion operator+(decimal s, const quaternion& q);
quaternion operator-(const quaternion& q, decimal s);
quaternion operator-(decimal s, const quaternion& q);
quaternion operator*(const quaternion& q, decimal s);
quaternion operator*(decimal s, const quaternion& q);
quaternion operator/(const quaternion& q, decimal s);
quaternion operator/(decimal s, const quaternion& q);

quaternion operator+(const quaternion& q1, const quaternion& q2);
quaternion operator-(const quaternion& q1, const quaternion& q2);
quaternion operator*(const quaternion& q1, const quaternion& q2);
quaternion operator/(const quaternion& q1, const quaternion& q2);

quaternion operator*(const quaternion& q, const vector3d& v);
quaternion operator*(const vector3d& v, quaternion& q);

//Rotaciona o quat�rnion q2 pelo quat�rnion q1
quaternion quaternion_rotate(const quaternion& q1, const quaternion& q2);
//Rotaciona o vector3d v pelo quat�rnion q
vector3d quaternion_vector_rotate(const quaternion& q, const vector3d& v);
//Constr�i um quat�rnion a partir de �ngulos de Euler.
quaternion quaternion_from_euler_angles(decimal x, decimal y, decimal z);
//Extrai os �ngulos de Euler do quat�rnion.
vector3d quaternion_extract_euler_angles(const quaternion& q);
quaternion quaternion_align_axis(const vector3d& v1, const vector3d& v2);
}
#endif
