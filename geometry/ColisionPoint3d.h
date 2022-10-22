

#ifndef COLISIONPOINT3D_H
#define COLISIONPOINT3D_H


namespace gpp
{

class ColisionPoint3d
{
public:
static bool isColiding(const vector3d& v1, const vector3d& v3);
static bool isColiding(sphere3d* c, const vector3d& v);
static bool isColiding(box3d* b, const vector3d& v);
static bool isColiding(line3d* l, const vector3d& v);
};
}
#endif
