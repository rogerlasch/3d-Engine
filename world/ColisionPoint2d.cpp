

#include"geometry.h"
#include"ColisionPoint2d.h"

namespace gpp
{
/**
*Colisão de ponto a ponto, verifica-se apenas se todas as coordenadas são iguais.
**/
bool ColisionPoint2d::isColiding(const vector3d& v1, const vector3d& v2)
{
return v1.x==v2.x&&v1.y==v2.y&&v2.z==v1.z;
}

/**
*Colisão entre círculo e ponto, verifica-se se a distância entre o ponto e o círculo é menor ou igual ao raio do círculo.
**/
bool ColisionPoint2d::isColiding(circle2d* c, const vector3d& v)
{
float dist=c->position.get_distance(v);
return dist<=c->radius;
}

/**
*Caixa contra ponto é fácil. Basta verificar se o ponto está dentro dos limites da caixa.
*Note que a caixa pode ser tanto um retângulo como um quadrado.
**/
bool ColisionPoint2d::isColiding(box2d* b, const vector3d& v)
{
return ((b->min.x<=v.x)&&(b->max.x>=v.x)&&(b->min.y<=v.y)&&(b->max.y>=v.y));
}

/**
*Colisão entre linha e ponto, primeiro calcula-se a distância entre o ponto, e as duas extremidades da linha.
*Em seguida, calcula-se o comprimento da linha. Ou seja, distância entre os 2 extremos.
*Então, verifica-se se d1+d2 é menor ou igual ao comprimento da linha.
*No teste, adiciona-se um intervalo de teste para efeitos de arredondamento de valores.
*Aqui verificamos se d1+d2 é maior ou igual ao comprimento da linha - 0.1, e se d1+d2 é menor ou igual ao comprimento da linha + 0.1
**/
bool ColisionPoint2d::isColiding(line2d* l, const vector3d& v)
{
float dist1=l->min.get_distance(v);
float dist2=l->max.get_distance(v);
float line_len=l->min.get_distance(l->max);
return (((dist1+dist2)>=(line_len-0.1f))&&((dist1+dist2)<=(line_len+0.1f)));
}
}
