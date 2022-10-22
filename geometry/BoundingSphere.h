

#ifndef BOUNDINGSPHERE_H
#define BOUNDINGSPHERE_H

namespace gpp
{

class BoundingSphere : public aabb
{
public:
vector3d center;
float radius;
BoundingSphere(const vector3d& min={0.0f, 0.0f, 0.0f}, float radius=0.0f);
BoundingSphere(const BoundingSphere& ab)=delete;
BoundingSphere& operator=(const BoundingSphere& ab)=delete;
virtual ~BoundingSphere();
virtual void translate(const vector3d& v);
virtual void scale(float sk);
virtual void recalculate();
};
}
#endif
