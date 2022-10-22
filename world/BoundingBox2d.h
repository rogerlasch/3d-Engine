

#ifndef BOUNDINGBOX2D_H
#define BOUNDINGBOX2D_H

namespace gpp
{

class BoundingBox2d
{
public:
vector3d m1;
vector3d m2;
vector3d m3;
vector3d m4;
vector3d center;
vector3d measures;
BoundingBox2d();
BoundingBox2d(const vector3d& m1, const vector3d& m2, const vector3d& m3, const vector3d& m4);
BoundingBox2d(const BoundingBox2d& bb);
BoundingBox2d& operator=(const BoundingBox2d& bb);
void reset();
void recalculate();
bool isColiding(BoundingBox2d* b);
bool canContains(BoundingBox2d* b);
bool isContained(BoundingBox2d* b);
void translate(const vector3d& v);
void scale(float sc);
std::string toString()const;
};
}
#endif
