

#ifndef SPHERE3D_H
#define SPHERE3D_H

namespace gpp
{


class sphere3d : public gpp_shape
{
public:
vector3d center;
float radius;
sphere3d(const vector3d& center={0.0f, 0.0f, 0.0f},float radius=1.0f);
sphere3d(const sphere3d& b);
sphere3d& operator=(const sphere3d& b);
virtual ~sphere3d();
virtual float getVolume();
virtual vector3d getCenter();
virtual void scale(float sk);
virtual void translate(const vector3d& v);
virtual void rotate(float angle, const vector3d& origin);
};
}
#endif
