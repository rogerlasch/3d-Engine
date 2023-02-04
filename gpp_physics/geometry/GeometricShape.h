


#ifndef GEOMETRICSHAPE_H
#define GEOMETRICSHAPE_H

namespace gpp
{

//Define os tipos de geometria que teremos...
enum GppShapeTypes : uint32
{
GTYPE_DEFAULT=0,
GTYPE_BOX=(1<<0),
GTYPE_SPHERE=(1<<1),
GTYPE_LINE=(1<<2),
GTYPE_CYLINDER=(1<<3)
};

class GeometricShape {
private:
uint32 geometricType;
public:
GeometricShape(uint32 geometricType) : geometricType(geometricType){}
virtual ~GeometricShape()=default;
uint32 GetGeometricType()const {return geometricType;}
virtual vector3d GetCenter()const=0;//Retorna o centro da geometria...
  virtual bool Contains(const vector3d& point) const = 0;//Retorna true se o ponto está dentro da geometria...
  virtual void Translate(const vector3d& translation) = 0;//Translada a geometria...
  virtual void Scale(float scale) = 0;//Escala a geometria...
virtual void Scale(const vector3d& scale)=0;//Escala a geometria...
virtual void Rotate(const quaternion& orientation)=0;
  virtual vector3d ClosestPointOnSurface(const vector3d& point) const = 0;//Retorna o ponto mais próximo na superfície da geometria...
  virtual float Volume() const = 0;//Retorna o volume da geometria...
virtual matrix3x3 GetInertiaTensor(float mass)const=0;//Calcula a matriz de inérsia da geometria.
friend class collision3d;
};
}
#endif
