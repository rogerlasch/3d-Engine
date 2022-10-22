

#include"geometry.h"
#include"ColisionLine3d.h"

namespace gpp
{
/**
*Ainda está confuso esse algoritmo...
*Linha contra linha.
**/
bool ColisionLine3d::isColiding(line3d* l1, line3d* l2)
{
float f1=0.0f, f2=0.0f;
vector3d v1, v2;
return ClosestPtSegmentSegment(l1->min, l1->max, l2->min, l2->max, f1, f2, v1, v2)<=0.5f;
}

/**
*Círculo contra linha.
*Primeiro verificar se alguma das pontas da linha está dentro do círculo.
*Depois calcular o ponto mais próximo do círculo.
*E por último, fazer a colisão de círculo e ponto.
**/
bool ColisionLine3d::isColiding(sphere3d* c, line3d* l)
{
if((ColisionPoint3d::isColiding(c, l->min))||(ColisionPoint3d::isColiding(c, l->max)))
{
return true;
}
float line_length=vector3d::get_distance(l->min, l->max);
float dot=(((c->center.x-l->min.x)*(l->max.x-l->min.x))+((c->center.y-l->min.y)*(l->max.y-l->min.y))+((c->center.z-l->min.z)*(l->max.z-l->min.z)))/pow(line_length,2);
vector3d v;
v.x=l->min.x+(dot*(l->max.x-l->min.x));
v.y=l->min.y + (dot * (l->max.y-l->min.y));
v.z=l->min.z + (dot * (l->max.z-l->min.z));
if(!ColisionPoint3d::isColiding(l, v))
{
return false;
}
return ColisionPoint3d::isColiding(c, v);
}

/**
*Linha e caixa...
*Monte as linhas e use o algoritmo de colisão entre linhas para determinar...
**/
bool ColisionLine3d::isColiding(box3d* b, line3d* l)
{
vector3d max(b->min+b->measures);
line3d left(b->min, vector3d(b->min.x, max.y));
line3d right(vector3d(max.x, b->min.y), max);
line3d top(left.max, right.max);
line3d bottom(left.min, right.min);
return ((isColiding(l, &left))||(isColiding(l, &right))||(isColiding(l, &top))||(isColiding(l, &bottom)));
}
}
