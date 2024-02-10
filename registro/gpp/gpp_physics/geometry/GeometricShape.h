
/**
*Interface que define nossa geometria básica.
**/

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
GTYPE_CYLINDER=(1<<2),
GTYPE_CAPSULE=(1<<3)
};

class GeometricShape {
private:
uint32 geometricType;
protected:
vector3d lastPosition;
public:
GeometricShape(uint32 geometricType) : geometricType(geometricType){}
virtual ~GeometricShape()=default;
virtual std::string toString()const=0;
uint32 getGeometricType()const {return geometricType;}
virtual float getProjectedArea()const=0;
virtual vector3d getCenter()const=0;//Retorna o centro da geometria...
  virtual void translate(const vector3d& translation, bool correction=false) = 0;//Translada a geometria...
  virtual void scale(const vector3d& origin, float s) = 0;//Escala a geometria...
virtual void scale(const vector3d& origin, const vector3d& sc)=0;//Escala a geometria...
virtual void rotate(const vector3d& origin, const quaternion& orientation)=0;
virtual matrix3x3 getInertiaTensor(float mass)const=0;//Calcula a matriz de inérsia da geometria.
inline vector3d getLastPosition()const {return this->lastPosition;}
inline void setLastPosition(const vector3d& v){this->lastPosition=v;}
friend class collision3d;
};
}
#endif
