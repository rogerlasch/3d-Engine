

#ifndef AABB_H
#define AABB_H

namespace gpp
{

enum AABB_TYPES
{
AABB_DEFAULT=0,
AABB_BOX,
AABB_SPHERE
};
class aabb
{
private:
uint32 atype;
protected:
shape* sh;
public:
aabb(uint32 type=AABB_DEFAULT);
aabb(const aabb& ab)=delete;
aabb& operator=(const aabb& ab)=delete;
virtual ~aabb();
uint32 getType()const;
bool isBoundingBox()const;
bool isBoundingSphere()const;
void setShape(shape* sh);
shape* getShape()const;
virtual void translate(const vector3d& v);
virtual void scale(float sk);
virtual void recalculate();
};
}
#endif
