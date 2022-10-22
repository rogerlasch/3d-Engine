

#ifndef SHAPE2D_H
#define SHAPE2d_H

namespace gpp
{

enum SHAPE_2DTYPE
{
SD2_GENERIC=0,
SD2_BOX,
SD2_CIRCLE,
SD2_LINE,
SD2_CONE,
SD2_ELIPSE,
SD2_CONVEX
};

class shape2d : public shape
{
public:
 shape2d(int gtype);
shape2d(const shape2d& ch)=delete;
shape2d& operator=(const shape2d& ch)=delete;
 virtual ~shape2d();
virtual float getArea();
virtual float getVolume();
virtual float getSurfaceArea();
virtual BoundingBox2d getBoundingBox()const;
virtual vector3d getCenter();
virtual bool is2d()const;
virtual bool is3d()const;
virtual void translate(const vector3d& v);
virtual void rotate(float angle, const vector3d& origin);
virtual bool isColiding(shape2d* ch);
};
}
#endif
