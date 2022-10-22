

#ifndef COLISIONLINE2D_H
#define COLISIONLINE2D_H


namespace gpp
{

class ColisionLine2d
{
public:
static bool isColiding(line2d* l1, line2d* l2);
static bool isColiding(circle2d* c, line2d* l);
static bool isColiding(box2d* b, line2d* l);
};
}
#endif
