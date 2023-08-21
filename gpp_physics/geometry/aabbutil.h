

/**
*Apenas algumas funções úteis para trabalhar com AABBS.
*Existem várias formas de se representar um AABB. Uma delas é calcular o ponto central, e calcular as meias medidas do centro até as bordas. Desta forma é boa porque economizamos a memória de alguns floats...
*Estas funções são mais utilizadas na classe octhree...
**/
#ifndef AABBUTIL_H
#define AABBUTIL_H

namespace gpp
{

//Funções para verificar se 2 AABBS se sobrepõem.
//A primeira versão assume o sistema que temos o ponto central do aab, e a meia medida do comprimento.
bool aabbOverlap(const vector3d& v1, float r1, const vector3d& v2, float r2);
bool aabbOverlap(const vector3d& center, float radius, AABB* b);
bool aabbOverlap(AABB* a, AABB* b);
//Verificações se um AABB é capaz de conter totalmente outro AABB. Útil nos nós da árvore octhree
bool aabbInsideAll(const vector3d& v1, float r1, const vector3d& v2, float r2);
bool aabbInsideAll(const vector3d& center, float radius, AABB* b);
bool aabbInsideAll(AABB* a, AABB* b);
bool aabbInsidePoint(AABB* a, const vector3d& v);
vector3d get_closest_point_from_aabb(const vector3d& pt, const vector3d& min, const vector3d& max);
bool point_in_aabb(const vector3d& pt, const vector3d& min, const vector3d& max);
}
#endif
