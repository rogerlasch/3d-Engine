
#include"geometry.h"
#include"colision2d.h"

using namespace std;

namespace gpp
{
 bool colision2d::isColiding(box2d* b1, box2d* b2)
{
vector3d m1((b1->min+b1->measures));
vector3d m2((b2->min+b2->measures));
vector3d v1=(b1->min-m2);
vector3d v2=(b2->min-m1);
if((v1.x>0.0f)||(v1.y>0.0f))
{
return false;
}
if((v2.x>0.0f)||(v2.y>0.0f))
{
return false;
}
return true;
}

 bool colision2d::isColiding(box2d* b, circle2d* c)
{
vector3d v;
if(c->position.x<b->min.x)
{
v.x=b->min.x;
}
else if(c->position.x>b->max.x)
{
v.x=b->max.x;
}
if(c->position.y<b->min.y)
{
v.y=b->min.y;
}
else if(c->position.y>b->max.y)
{
v.y=b->max.y;
}
float dist=c->position.get_distance(v);
return dist<=c->radius;
}

 bool colision2d::isColiding(box2d* b, line2d* l)
{
return ColisionLine2d::isColiding(b, l);
}

 bool colision2d::isColiding(circle2d* c1, circle2d* c2)
{
vector3d v((c1->position-c2->position));
v.x=v.x*v.x;
v.y=v.y*v.y;
float dist=(v.x+v.y);
float radius=(c1->radius+c2->radius);
radius=radius*radius;
return dist<=radius;
}

 bool colision2d::isColiding(circle2d* c, box2d* b)
{
return isColiding(b, c);
}

 bool colision2d::isColiding(circle2d* c, line2d* l)
{
return ColisionLine2d::isColiding(c, l);
}

 bool colision2d::isColiding(line2d* c1, line2d* c2)
{
float v1 = ((c2->max.x-c2->min.x)*(c1->min.y-c2->min.y) - (c2->max.y-c2->min.y)*(c1->min.x-c2->min.x)) / ((c2->max.y-c2->min.y)*(c1->max.x-c1->min.x) - (c2->max.x-c2->min.x)*(c1->max.y-c1->min.y));
float v2 = ((c1->max.x-c1->min.x)*(c1->min.y-c2->min.y) - (c1->max.y-c1->min.y)*(c1->min.x-c2->min.x)) / ((c2->max.y-c2->min.y)*(c1->max.x-c1->min.x) - (c2->max.x-c2->min.x)*(c1->max.y-c1->min.y));
return ((v1>=0)&&(v1<=1)&&(v2>=0)&&(v2<=1));
}

 bool colision2d::isColiding(line2d* l, box2d* b)
{
return ColisionLine2d::isColiding(b, l);
}

 bool colision2d::isColiding(line2d* l, circle2d* c)
{
return ColisionLine2d::isColiding(c, l);
}
}
