
/**
*Classe que representa um AABB (Aligned AXIS Bounding Box)
*Um AABB é uma caixa que envolve completamente uma forma geométrica mais complexa.
*Por exemplo, imagine uma esfera dentro de uma caixa quadrada. Este tipo de estrutura é utilizado para fazer uma primeira varredura barata de colisão.
*Um poliedro de N vértices é muito caro para ser testado toda vez contra uma esfera a cada quadro da simulação.
*Então, para evitarmos isto, "encaixotamos" o poliedro dentro de uma caixa e a
*esfera também, e caso as duas caixas estejam se sobrepondo, então o teste mais caro de detecção de colisão entre esfera e poliedro é executado.
*Um teste caixa contra caixa é muito barato computacionalmente e também muito preciso.
*Nós poderiamos encapsular nossas geometrias dentro de esferas também, mas, infelizmente, elas não são tão precisas como as caixas, o que pode acarretar em muitos falços positivos.
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
