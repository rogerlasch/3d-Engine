
#ifndef CAPSULE3D_H
#define CAPSULE3D_H

namespace gpp
{


class capsule3d : public GeometricShape
{
public:
vector3d min;
vector3d max;
float radius;
capsule3d(const vector3d& min={0.0f, 0.0f, 0.0f}, const vector3d& max={0.0f, 0.0f, 0.0f}, float radius=1.0f);
capsule3d(const capsule3d& b);
capsule3d& operator=(const capsule3d& b);
virtual ~capsule3d();
virtual std::string toString()const;
virtual float getProjectedArea()const;
virtual vector3d getCenter()const;
  virtual void translate(const vector3d& translation, bool correction=false);
  virtual void scale(const vector3d& origin, float s);
virtual void scale(const vector3d& origin, const vector3d& sc);
virtual void rotate(const vector3d& origin, const quaternion& orientation);
virtual matrix3x3 getInertiaTensor(float mass)const;
};
}
#endif
