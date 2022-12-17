

#ifndef GPP_SHAPE_H
#define GPP_SHAPE_H

namespace gpp
{

enum GPP_SHAPES : uint32
{
GTYPE_DEFAULT=0,
GTYPE_SPHERE=(1<<0),
GTYPE_BOX=(1<<1),
GTYPE_CAPSULE=(1<<2),
GTYPE_CYLINDER=(1<<3),
GTYPE_CONE=(1<<4),
GTYPE_POLYHEDRON=(1<<5),
GTYPE_LINE=(1<<6)
};

class gpp_shape
{
protected:
uint32 gtype;
public:
gpp_shape(uint32 gtype=GTYPE_DEFAULT);
virtual ~gpp_shape();
uint32 getGType()const;
virtual float getVolume()=0;
virtual vector3d getCenter()=0;
virtual void scale(float sk)=0;
virtual void translate(const vector3d& v)=0;
virtual void rotate(float angle, const vector3d& origin)=0;
friend class collision3d;
};
template<class T>
inline void gpp_copy_shape(gpp_shape* sh, const T& value)
{
T* sh2=(T*)sh;
*sh2=value;
}
gpp_shape* shape_create_from_type(uint32 type, const vector3d& vtranslate={0.0f, 0.0f, 0.0f});
}
#endif
