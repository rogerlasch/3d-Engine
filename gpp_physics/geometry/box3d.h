
/**
*Classe que representa uma caixa, ou cubo...
*Muito semelhante ao AABB.
**/
#ifndef BOX3D_H
#define BOX3D_H

namespace gpp
{


class box3d : public GeometricShape
{
public:
vector3d min;
vector3d max;
box3d(const vector3d& min={0.0f, 0.0f, 0.0f}, const vector3d& measures={0.0f, 0.0f, 0.0f});
box3d(const box3d& b);
box3d& operator=(const box3d& b);
virtual ~box3d();
virtual std::string toString()const;
virtual vector3d getCenter()const;//Retorna o centro da geometria...
void GetVertices(std::vector<vector3d>& vertices);
  virtual void translate(const vector3d& translation);//Translada a geometria...
  virtual void scale(float s);//Escala a geometria...
virtual void scale(const vector3d& sc);//Escala a geometria...
virtual void rotate(const quaternion& orientation);
virtual matrix3x3 getInertiaTensor(float mass)const;//Calcula a matriz de inérsia da geometria.
};
}
#endif
