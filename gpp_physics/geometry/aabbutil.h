

/**
*Apenas algumas fun��es �teis para trabalhar com AABBS.
*Existem v�rias formas de se representar um AABB. Uma delas � calcular o ponto central, e calcular as meias medidas do centro at� as bordas. Desta forma � boa porque economizamos a mem�ria de alguns floats...
*Estas fun��es s�o mais utilizadas na classe octhree...
**/
#ifndef AABBUTIL_H
#define AABBUTIL_H

namespace gpp
{

//Fun��es para verificar se 2 AABBS se sobrep�em.
//A primeira vers�o assume o sistema que temos o ponto central do aab, e a meia medida do comprimento.
bool aabbOverlap(const vector3d& v1, float r1, const vector3d& v2, float r2);
bool aabbOverlap(const vector3d& center, float radius, AABB* b);
bool aabbOverlap(AABB* a, AABB* b);
//Verifica��es se um AABB � capaz de conter totalmente outro AABB. �til nos n�s da �rvore octhree
bool aabbInsideAll(const vector3d& v1, float r1, const vector3d& v2, float r2);
bool aabbInsideAll(const vector3d& center, float radius, AABB* b);
bool aabbInsideAll(AABB* a, AABB* b);
bool aabbInsidePoint(AABB* a, const vector3d& v);
vector3d get_closest_point_from_aabb(const vector3d& pt, const vector3d& min, const vector3d& max);
bool point_in_aabb(const vector3d& pt, const vector3d& min, const vector3d& max);
}
#endif
