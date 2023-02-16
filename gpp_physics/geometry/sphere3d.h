
/**
*Classe que representa uma esfera.
*Uma esfera é formada  por um ponto central, e um raio.
**/
#ifndef SPHERE3D_H
#define SPHERE3D_H

namespace gpp
{


class sphere3d : public GeometricShape
{
public:
vector3d center;
float radius;
sphere3d(const vector3d& center={0.0f, 0.0f, 0.0f},float radius=1.0f);
sphere3d(const sphere3d& b);
sphere3d& operator=(const sphere3d& b);
virtual ~sphere3d();
virtual vector3d GetCenter()const;
  virtual bool Contains(const vector3d& point) const;
  virtual void Translate(const vector3d& translation);
  virtual void Scale(float scale);
virtual void Scale(const vector3d& scale);
virtual void Rotate(const quaternion& orientation);
  virtual vector3d ClosestPointOnSurface(const vector3d& point) const;
  virtual float Volume() const;
virtual matrix3x3 GetInertiaTensor(float mass)const;
};
}
#endif
