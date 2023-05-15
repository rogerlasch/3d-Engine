
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

typedef std::pair<RigidBody*, RigidBody*> CollisionPair;
typedef std::vector<CollisionPair> CollisionPairList;
class BroadPhase
{
uint32 gsortaxis;
public:
BroadPhase();
void scan(RigidBodyList& bodies, CollisionPairList& collisions);
void scan(RigidBodyList& bodies, CollisionPairList& collisions, std::shared_mutex& m);
uint32 getgSortAxis()const;
void setgSortAxis(uint32 gsortaxis);
};
}
#endif
