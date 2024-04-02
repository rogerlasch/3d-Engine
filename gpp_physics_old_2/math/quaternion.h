

/**
*Classe de quatérnion para a simulação.
*Esta classe também foi retirada do livro Physics for game developers.
*Vários métodos adicionais foram adicionados.
*Esta classe serve para armazenar a orientação de um corpo no espaço.
**/

#ifndef QUATERNION_H
#define QUATERNION_H

namespace gpp
{

/*
* Classe de quatérnions fortemente baseada na classe quaternion do livro Physics for Game Developers.
**/
class quaternion
{
public:
float x;
float y;
float z;
float w;
quaternion(float x=0.0f, float y=0.0f, float z=0.0f, float w=0.0f);
quaternion(float w, const vector3d& v);
quaternion(const quaternion& q);
quaternion& operator=(const quaternion& q);
bool operator==(const quaternion& q);
quaternion& operator+=(float s);
quaternion& operator-=(float s);
quaternion& operator*=(float s);
quaternion& operator/=(float s);
quaternion& operator+=(const quaternion& q);
quaternion& operator-=(const quaternion& q);
quaternion operator~()const;
void setX(float x);
float getX()const;
void setY(float y);
float getY()const;
void setZ(float z);
float getZ()const;
void setW(float w);
float getW()const;
//Calcula o comprimento do quatérnion...
float length()const;
//Método de conjugar o quatérnion...
quaternion conjugate()const;
//Normaliza o quaternion...
quaternion& normalize();
quaternion& setIdentity();
//Retorna o ângulo que o quatérnion está representando...
float getAngle()const;
vector3d getAxis()const;
std::string toString()const;
matrix3x3 toMatrix3x3()const;
};

//Sobrecarga dos operadores do quatérnion...

std::ostream& operator<<(std::ostream& os, const quaternion& q);
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

//Rotaciona o quatérnion q2 pelo quatérnion q1
quaternion quaternion_rotate(const quaternion& q1, const quaternion& q2);
//Rotaciona o vector3d v pelo quatérnion q
vector3d quaternion_vector_rotate(const quaternion& q, const vector3d& v);
//Constrói um quatérnion a partir de ângulos de Euler.
quaternion quaternion_from_euler_angles(float x, float y, float z);
//Extrai os ângulos de Euler do quatérnion.
vector3d quaternion_extract_euler_angles(const quaternion& q);
}
#endif
