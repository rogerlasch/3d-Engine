


#ifndef QUATERNION_H
#define QUATERNION_H

namespace gpp
{

/*
* Classe de quatérnions fortemente baseada na classe quaternion do livro Physics for Game Developers.
*Ela usa um vector3d para a sua parte vetorial.
**/
class quaternion
{
public:
//Escalar do quatérnion...
float n;
//Parte vetorial do quatérnion...
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
//Calcula o comprimento do quatérnion...
float length();
//Método de conjugar o quatérnion...
quaternion conjugate()const;
//Retorna o ângulo que o quatérnion está representando...
float getAngle()const;
vector3d getAxis()const;

};

//Sobrecarga dos operadores do quatérnion...
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
