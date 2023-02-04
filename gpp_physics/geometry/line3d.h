

#ifndef LINE3D_H
#define LINE3D_H

namespace gpp
{


class line3d : public GeometricShape
{
public:
vector3d min;
vector3d max;
line3d(const vector3d& min={0.0f, 0.0f, 0.0f}, const vector3d& max={0.0f, 0.0f, 0.0f});
line3d(const line3d& b);
line3d& operator=(const line3d& b);
virtual ~line3d();
virtual vector3d GetCenter()const;//Retorna o centro da geometria...
  virtual bool Contains(const vector3d& point) const;//Retorna true se o ponto está dentro da geometria...
  virtual void Translate(const vector3d& translation);//Translada a geometria...
  virtual void Scale(float scale);//Escala a geometria...
virtual void Scale(const vector3d& scale);//Escala a geometria...
virtual void Rotate(const quaternion& orientation);
  virtual vector3d ClosestPointOnSurface(const vector3d& point) const;//Retorna o ponto mais próximo na superfície da geometria...
  virtual float Volume() const;//Retorna o volume da geometria...
virtual matrix3x3 GetInertiaTensor(float mass)const;//Calcula a matriz de inérsia da geometria.
};
}
#endif
