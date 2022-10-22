

#ifndef COLISION2D_H
#define COLISION2D_H

namespace gpp
{

class colision2d
{
public:
static bool isColiding(box2d* b1, box2d* b2);
static bool isColiding(box2d* b, circle2d* c);
static bool isColiding(box2d* b, line2d* l);
static bool isColiding(circle2d* c1, circle2d* c2);
static bool isColiding(circle2d* c, box2d* b);
static bool isColiding(circle2d* c, line2d* l);
static bool isColiding(line2d* l1, line2d* l2);
static bool isColiding(line2d* l, box2d* b);
static bool isColiding(line2d* l, circle2d* c);
};

struct Shape2dConverter
{
template<class T>
bool operator()(T* c1, shape2d* c2)
{
switch(c2->getGtype())
{
case SD2_BOX:
{
return colision2d::isColiding(c1, (box2d*)c2);
}
case SD2_CIRCLE:
{
return colision2d::isColiding(c1, (circle2d*)c2);
}
case SD2_LINE:
{
return colision2d::isColiding(c1, (line2d*)c2);
}
}
return false;
}
};
}
#endif
