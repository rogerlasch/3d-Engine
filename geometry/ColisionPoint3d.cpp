

#include"geometry.h"
#include"ColisionPoint3d.h"

namespace gpp
{
/**
*Colis�o de ponto a ponto, verifica-se apenas se todas as coordenadas s�o iguais.
**/
bool ColisionPoint3d::isColiding(const vector3d& v1, const vector3d& v2)
{
return v1.x==v2.x&&v1.y==v2.y&&v2.z==v1.z;
}

/**
*Colis�o entre c�rculo e ponto, verifica-se se a dist�ncia entre o ponto e o c�rculo � menor ou igual ao raio do c�rculo.
**/
bool ColisionPoint3d::isColiding(sphere3d* c, const vector3d& v)
{
float dist=vector3d::get_distance(c->center, v);
return dist<=c->radius;
}

/**
*Caixa contra ponto � f�cil. Basta verificar se o ponto est� dentro dos limites da caixa.
*Note que a caixa pode ser tanto um ret�ngulo como um quadrado.
**/
bool ColisionPoint3d::isColiding(box3d* b, const vector3d& v)
{
vector3d max=(b->min+b->measures);
return ((b->min.x<=v.x)&&(max.x>=v.x)&&(b->min.y<=v.y)&&(max.y>=v.y)&&(b->min.z<=v.z)&&(max.z>=v.z));
}

/**
*Colis�o entre linha e ponto, primeiro calcula-se a dist�ncia entre o ponto, e as duas extremidades da linha.
*Em seguida, calcula-se o comprimento da linha. Ou seja, dist�ncia entre os 2 extremos.
*Ent�o, verifica-se se d1+d2 � menor ou igual ao comprimento da linha.
*No teste, adiciona-se um intervalo de teste para efeitos de arredondamento de valores.
*Aqui verificamos se d1+d2 � maior ou igual ao comprimento da linha - 0.1, e se d1+d2 � menor ou igual ao comprimento da linha + 0.1
**/
bool ColisionPoint3d::isColiding(line3d* l, const vector3d& v)
{
float dist1=vector3d::get_distance(l->min, v);
float dist2=vector3d::get_distance(l->max, v);
float line_len=vector3d::get_distance(l->min, l->max);
return (((dist1+dist2)>=(line_len-0.1f))&&((dist1+dist2)<=(line_len+0.1f)));
}
}
