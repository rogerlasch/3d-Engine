

#ifndef COLISIONPOINT2D_H
#define COLISIONPOINT2D_H


namespace gpp
{

class ColisionPoint2d
{
public:
static bool isColiding(const vector3d& v1, const vector3d& v2);
static bool isColiding(circle2d* c, const vector3d& v);
static bool isColiding(box2d* b, const vector3d& v);
static bool isColiding(line2d* l, const vector3d& v);
};
}
#endif
