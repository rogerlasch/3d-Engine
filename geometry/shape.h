

#ifndef SHAPE_H
#define SHAPE_H

namespace gpp
{

enum GPP_SHAPES
{
GTYPE_DEFAULT=0,
GTYPE_SPHERE,
GTYPE_BOX,
GTYPE_CAPSULE,
GTYPE_CONE,
GTYPE_POLYGON,
GTYPE_LINE
};

class shape
{
protected:
uint32 gtype;
public:
shape(uint32 gtype=GTYPE_DEFAULT);
virtual ~shape();
uint32 getGtype()const;
virtual float getVolume()=0;
virtual vector3d getCenter()=0;
virtual void scale(float sk)=0;
virtual void translate(const vector3d& v)=0;
virtual void rotate(float angle, const vector3d& origin)=0;
};
}
#endif
