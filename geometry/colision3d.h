

#ifndef COLISION3D_H
#define COLISION3D_H

namespace gpp
{

class colision3d
{
public:
static bool isColiding(box3d* b1, box3d* b2);
static bool isColiding(box3d* b, sphere3d* c);
static bool isColiding(box3d* b, line3d* l);
static bool isColiding(sphere3d* c1, sphere3d* c2);
static bool isColiding(sphere3d* c, box3d* b);
static bool isColiding(sphere3d* c, line3d* l);
static bool isColiding(line3d* l1, line3d* l2);
static bool isColiding(line3d* l, box3d* b);
static bool isColiding(line3d* l, sphere3d* c);
};

struct Shape3dConverter
{
template<class T>
bool operator()(T* c1, shape* c2)
{
switch(c2->getGtype())
{
case GTYPE_BOX:
{
return colision3d::isColiding(c1, (box3d*)c2);
}
case GTYPE_SPHERE:
{
return colision3d::isColiding(c1, (sphere3d*)c2);
}
case GTYPE_LINE:
{
return colision3d::isColiding(c1, (line3d*)c2);
}
}
return false;
}
};
}
#endif
