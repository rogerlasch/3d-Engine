

#ifndef COLLISION3D_H
#define COLLISION3D_H

namespace gpp
{

class collision3d
{
public:
static bool isColliding(gpp_shape* s1, gpp_shape* s2);
static bool isColliding(gpp_shape* sh, const vector3d& v);
static bool sphereSphere(sphere3d* s1, sphere3d* s2);
static bool sphereLine(sphere3d* s, line3d* l);
static bool sphereBox(sphere3d* s, box3d* b);
static bool boxBox(box3d* b1, box3d* b2);
static bool boxLine(box3d* b, line3d* l);
static bool lineLine(line3d* l1, line3d* l2);
//Auxiliary, don't use it!
static bool spherePoint(sphere3d* s, const vector3d& v);
static bool boxPoint(box3d* b, const vector3d& v);
static bool linePoint(line3d* l, const vector3d& v);
};
}
#endif
