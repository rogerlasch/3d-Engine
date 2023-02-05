

#ifndef AABB_H
#define AABB_H

namespace gpp
{

class AABB
{
public:
vector3d min;
vector3d max;
GeometricShape* shape;
public:
AABB(const vector3d& min={0.0f, 0.0f, 0.0f}, const vector3d& max={0.0f, 0.0f, 0.0f}, GeometricShape* sh=NULL);
AABB(const AABB& ab)=delete;
AABB& operator=(const AABB& ab)=delete;
virtual ~AABB();
void setShape(GeometricShape* sh);
GeometricShape* getGeometricShape()const;
void setMin(const vector3d& m);
vector3d getMin()const;
void setMax(const vector3d& m);
vector3d getMax()const;
virtual void translate(const vector3d& v);
virtual void scale(float sk);
virtual void recalculateBoundingBox();
virtual std::string toString()const;
friend class BroadPhase;
};
}
#endif
