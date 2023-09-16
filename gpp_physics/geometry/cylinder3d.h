
#ifndef CYLINDER3D_H
#define CYLINDER3D_H

namespace gpp
{


class cylinder3d : public GeometricShape
{
public:
vector3d min;
vector3d max;
float radius;
cylinder3d(const vector3d& min={0.0f, 0.0f, 0.0f}, const vector3d& max={0.0f, 0.0f, 0.0f}, float radius=1.0f);
cylinder3d(const cylinder3d& b);
cylinder3d& operator=(const cylinder3d& b);
virtual ~cylinder3d();
virtual std::string toString()const;
virtual vector3d getCenter()const;
  virtual void translate(const vector3d& translation);
  virtual void scale(const vector3d& origin, float s);
virtual void scale(const vector3d& origin, const vector3d& sc);
virtual void rotate(const vector3d& origin, const quaternion& orientation);
virtual matrix3x3 getInertiaTensor(float mass)const;
};
}
#endif
