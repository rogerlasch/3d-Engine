

#ifndef BOX3D_H
#define BOX3D_H

namespace gpp
{


class box3d : public GeometricShape
{
public:
vector3d min;
vector3d measures;
box3d(const vector3d& min={0.0f, 0.0f, 0.0f}, const vector3d& measures={0.0f, 0.0f, 0.0f});
box3d(const box3d& b);
box3d& operator=(const box3d& b);
virtual ~box3d();
virtual vector3d GetCenter()const;//Retorna o centro da geometria...
void GetVertices(std::vector<vector3d>& vertices);
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
