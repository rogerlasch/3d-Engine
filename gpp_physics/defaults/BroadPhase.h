
/**
*Classe que implementa o algoritmo de broadphase do nosso simulador.
*Nesta fase, dada uma lista de corpos, o objetivo � retornar uma lista com todos os pares que possivelmente estejam colidindo.
*Nesta etapa, um teste de sobreposi��o aabb x aabb � feito.
*Caso uma sobreposi��o seja detectada, o par de corpos � adicionado a lista de sa�da para uma verifica��o mais precisa e de geometria no algoritmo contido na classe NarrowPhase.
**/

#ifndef BROADPHASE_H
#define BROADPHASE_H


namespace gpp
{

class BroadPhase : public iBroadPhase
{
uint32 gsortaxis;
public:
BroadPhase();
virtual void scan(std::vector<iRigidBody*>& bodies, std::vector<CollisionInfo>& collisions);
uint32 getgSortAxis()const;
void setgSortAxis(uint32 gsortaxis);
};
}
#endif
