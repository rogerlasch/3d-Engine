

#ifndef LINE2D_H
#define LINE2D_H

namespace gpp
{

class line2d : public shape2d
{
public:
vector3d min;
vector3d max;
line2d(const vector3d& min={0.0f, 0.0f}, const vector3d& max={0.0f, 0.0f});
line2d(const line2d& ld);
line2d& operator=(const line2d& ld);
virtual ~line2d();
virtual float getArea();
virtual vector3d getCenter();
virtual void translate(const vector3d& v);
virtual void rotate(float angle, const vector3d& origin);
virtual bool isColiding(shape2d* sh);
};
};
#endif
