

#ifndef CIRCLE2D_H
#define CIRCLE2D_H

namespace gpp
{

class circle2d : public shape2d
{
public:
vector3d position;
float radius;
circle2d(const vector3d& position={0.0f, 0.0f, 0.0f}, float radius=0.0f);
circle2d(const circle2d& c);
circle2d& operator=(const circle2d& c);
virtual ~circle2d();
virtual float getArea();
vector3d getCenter();
virtual BoundingBox2d getBoundingBox()const;
virtual void translate(const vector3d& v);
virtual void rotate(float angle, const vector3d& origin);
virtual bool isColiding(shape2d* ch);
};
}
#endif
