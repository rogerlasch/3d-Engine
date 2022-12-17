

#ifndef BOX3D_H
#define BOX3D_H

namespace gpp
{


class box3d : public gpp_shape
{
public:
vector3d min;
vector3d measures;
box3d(const vector3d& min={0.0f, 0.0f, 0.0f}, const vector3d& measures={0.0f, 0.0f, 0.0f});
box3d(const box3d& b);
box3d& operator=(const box3d& b);
virtual ~box3d();
virtual float getVolume();
virtual vector3d getCenter();
virtual void scale(float sk);
virtual void translate(const vector3d& v);
virtual void rotate(float angle, const vector3d& origin);
};
}
#endif
