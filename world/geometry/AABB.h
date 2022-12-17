

#ifndef AABB_H
#define AABB_H

namespace gpp
{

enum AABB_INSIDE_VALUES
{
AABB_INSIDE_ALL=0,
AABB_INSIDE_PARTIAL,
AABB_OUT
};

class AABB
{
public:
vector3d min;
vector3d max;
gpp_shape* shape;
public:
AABB(const vector3d& min={0.0f, 0.0f, 0.0f}, const vector3d& max={0.0f, 0.0f, 0.0f}, gpp_shape* sh=NULL);
AABB(const AABB& ab)=delete;
AABB& operator=(const AABB& ab)=delete;
virtual ~AABB();
float getVolume()const;
void setShape(uint32 gtype);
void setShape(gpp_shape* sh);
gpp_shape* getShape()const;
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
