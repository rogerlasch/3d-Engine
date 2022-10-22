

#include"geometry.h"
#include"ColisionLine2d.h"

namespace gpp
{
/**
*Ainda está confuso esse algoritmo...
*Linha contra linha.
**/
bool ColisionLine2d::isColiding(line2d* l1, line2d* l2)
{
float v1 = ((l2->max.x-l2->min.x)*(l1->min.y-l2->min.y) - (l2->max.y-l2->min.y)*(l1->min.x-l2->min.x)) / ((l2->max.y-l2->min.y)*(l1->max.x-l1->min.x) - (l2->max.x-l2->min.x)*(l1->max.y-l1->min.y));
float v2 = ((l1->max.x-l1->min.x)*(l1->min.y-l2->min.y) - (l1->max.y-l1->min.y)*(l1->min.x-l2->min.x)) / ((l2->max.y-l2->min.y)*(l1->max.x-l1->min.x) - (l2->max.x-l2->min.x)*(l1->max.y-l1->min.y));
return ((v1>=0)&&(v1<=1)&&(v2>=0)&&(v2<=1));
}

/**
*Círculo contra linha.
*Primeiro verificar se alguma das pontas da linha está dentro do círculo.
*Depois calcular o ponto mais próximo do círculo.
*E por último, fazer a colisão de círculo e ponto.
**/
bool ColisionLine2d::isColiding(circle2d* c, line2d* l)
{
if((ColisionPoint2d::isColiding(c, l->min))||(ColisionPoint2d::isColiding(c, l->max)))
{
return true;
}
float line_length=l->min.get_distance(l->max);
float dot=(((c->position.x-l->min.x)*(l->max.x-l->min.x))+((c->position.y-l->min.y)*(l->max.y-l->min.y)))/pow(line_length,2);
vector3d v;
v.x=l->min.x+(dot*(l->max.x-l->min.x));
v.y=l->min.y + (dot * (l->max.y-l->min.y));
if(!ColisionPoint2d::isColiding(l, v))
{
return false;
}
return ColisionPoint2d::isColiding(c, v);
}

/**
*Linha e caixa...
*Monte as linhas e use o algoritmo de colisão entre linhas para determinar...
**/
bool ColisionLine2d::isColiding(box2d* b, line2d* l)
{
line2d left(b->min, vector3d(b->min.x, b->max.y));
line2d right(vector3d(b->max.x, b->min.y), b->max);
line2d top(left.max, right.max);
line2d bottom(left.min, right.min);
return ((isColiding(l, &left))||(isColiding(l, &right))||(isColiding(l, &top))||(isColiding(l, &bottom)));
}
}
