
/**
*Classe que implementa o algoritmo de broadphase do nosso simulador.
*Nesta fase, dada uma lista de corpos, o objetivo é retornar uma lista com todos os pares que possivelmente estejam colidindo.
*Nesta etapa, um teste de sobreposição aabb x aabb é feito.
*Caso uma sobreposição seja detectada, o par de corpos é adicionado a lista de saída para uma verificação mais precisa e de geometria no algoritmo contido na classe NarrowPhase.
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
