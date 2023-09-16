
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
