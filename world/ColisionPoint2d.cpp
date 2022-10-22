

#include"geometry.h"
#include"ColisionPoint2d.h"

namespace gpp
{
/**
*Colis�o de ponto a ponto, verifica-se apenas se todas as coordenadas s�o iguais.
**/
bool ColisionPoint2d::isColiding(const vector3d& v1, const vector3d& v2)
{
return v1.x==v2.x&&v1.y==v2.y&&v2.z==v1.z;
}

/**
*Colis�o entre c�rculo e ponto, verifica-se se a dist�ncia entre o ponto e o c�rculo � menor ou igual ao raio do c�rculo.
**/
bool ColisionPoint2d::isColiding(circle2d* c, const vector3d& v)
{
float dist=c->position.get_distance(v);
return dist<=c->radius;
}

/**
*Caixa contra ponto � f�cil. Basta verificar se o ponto est� dentro dos limites da caixa.
*Note que a caixa pode ser tanto um ret�ngulo como um quadrado.
**/
bool ColisionPoint2d::isColiding(box2d* b, const vector3d& v)
{
return ((b->min.x<=v.x)&&(b->max.x>=v.x)&&(b->min.y<=v.y)&&(b->max.y>=v.y));
}

/**
*Colis�o entre linha e ponto, primeiro calcula-se a dist�ncia entre o ponto, e as duas extremidades da linha.
*Em seguida, calcula-se o comprimento da linha. Ou seja, dist�ncia entre os 2 extremos.
*Ent�o, verifica-se se d1+d2 � menor ou igual ao comprimento da linha.
*No teste, adiciona-se um intervalo de teste para efeitos de arredondamento de valores.
*Aqui verificamos se d1+d2 � maior ou igual ao comprimento da linha - 0.1, e se d1+d2 � menor ou igual ao comprimento da linha + 0.1
**/
bool ColisionPoint2d::isColiding(line2d* l, const vector3d& v)
{
float dist1=l->min.get_distance(v);
float dist2=l->max.get_distance(v);
float line_len=l->min.get_distance(l->max);
return (((dist1+dist2)>=(line_len-0.1f))&&((dist1+dist2)<=(line_len+0.1f)));
}
}
