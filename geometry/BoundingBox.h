

#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

namespace gpp
{

class BoundingBox : public aabb
{
public:
vector3d min;
vector3d max;
BoundingBox(const vector3d& min={0.0f, 0.0f, 0.0f}, const vector3d& max={0.0f, 0.0f, 0.0f});
BoundingBox(const BoundingBox& ab)=delete;
BoundingBox& operator=(const BoundingBox& ab)=delete;
virtual ~BoundingBox();
virtual void translate(const vector3d& v);
virtual void scale(float sk);
virtual void recalculate();
};
}
#endif
