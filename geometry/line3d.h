

#ifndef LINE3D_H
#define LINE3D_H

namespace gpp
{


class line3d : public shape
{
public:
vector3d min;
vector3d max;
line3d(const vector3d& min={0.0f, 0.0f, 0.0f}, const vector3d& max={0.0f, 0.0f, 0.0f});
line3d(const line3d& b);
line3d& operator=(const line3d& b);
virtual ~line3d();
virtual float getVolume();
virtual vector3d getCenter();
virtual void scale(float sk);
virtual void translate(const vector3d& v);
virtual void rotate(float angle, const vector3d& origin);
};
}
#endif
