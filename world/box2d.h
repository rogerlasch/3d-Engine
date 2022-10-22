

#ifndef BOX2D_H
#define BOX2D_H

namespace gpp
{

class box2d : public shape2d
{
public:
vector3d min;
vector3d max;
vector3d measures;
box2d(const vector3d& position={0.0f, 0.0f, 0.0f}, const vector3d& measures={0.0f, 0.0f, 0.0f});
box2d(const box2d& b);
box2d& operator=(const box2d& b);
virtual ~box2d();
virtual float getArea();
virtual vector3d getCenter();
virtual BoundingBox2d getBoundingBox()const;
virtual void translate(const vector3d& v);
virtual void rotate(float angle, const vector3d& origin);
virtual bool isColiding(shape2d* ch);
};
}
#endif
