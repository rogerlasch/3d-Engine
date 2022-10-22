

#ifndef COLISIONLINE3D_H
#define COLISIONLINE3D_H


namespace gpp
{

class ColisionLine3d
{
public:
static bool isColiding(line3d* l1, line3d* l2);
static bool isColiding(sphere3d* c, line3d* l);
static bool isColiding(box3d* b, line3d* l);
};
}
#endif
