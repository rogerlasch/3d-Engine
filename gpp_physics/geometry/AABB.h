
/**
*Classe que representa um AABB (Aligned AXIS Bounding Box)
*Um AABB � uma caixa que envolve completamente uma forma geom�trica mais complexa.
*Por exemplo, imagine uma esfera dentro de uma caixa quadrada. Este tipo de estrutura � utilizado para fazer uma primeira varredura barata de colis�o.
*Um poliedro de N v�rtices � muito caro para ser testado toda vez contra uma esfera a cada quadro da simula��o.
*Ent�o, para evitarmos isto, "encaixotamos" o poliedro dentro de uma caixa e a
*esfera tamb�m, e caso as duas caixas estejam se sobrepondo, ent�o o teste mais caro de detec��o de colis�o entre esfera e poliedro � executado.
*Um teste caixa contra caixa � muito barato computacionalmente e tamb�m muito preciso.
*N�s poderiamos encapsular nossas geometrias dentro de esferas tamb�m, mas, infelizmente, elas n�o s�o t�o precisas como as caixas, o que pode acarretar em muitos fal�os positivos.
**/
#ifndef AABB_H
#define AABB_H

namespace gpp
{

class AABB
{
public:
vector3d min;
vector3d max;
GeometricShape* shape;
public:
AABB(const vector3d& min={0.0f, 0.0f, 0.0f}, const vector3d& max={0.0f, 0.0f, 0.0f}, GeometricShape* sh=NULL);
AABB(const AABB& ab)=delete;
AABB& operator=(const AABB& ab)=delete;
virtual ~AABB();
float getVolume()const;
void setGeometricShape(GeometricShape* sh);
GeometricShape* getGeometricShape()const;
void setMin(const vector3d& m);
vector3d getMin()const;
void setMax(const vector3d& m);
vector3d getMax()const;
virtual void translate(const vector3d& v);
virtual void scale(float sk);
virtual void rotate(const quaternion& orientation);
virtual void recalculateBoundingBox();
virtual std::string toString()const;
friend class BroadPhase;
};
}
#endif
