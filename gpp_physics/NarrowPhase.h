

/**
*Nesta classe é onde verificamos se 2 corpos estão de fato colidindo.
*Aqui é onde faremos os testes computacionalmente caros entre geometrias específicas como esfera x caixa, caixa x caixa, e etc...
*Caso uma colisão de fato esteja sendo detectada, ela deve adicionar o par de corpos bem como informações da colisão em uma lista para o solucionador de colisões resolver elas.
**/
#ifndef NARROWPHASE_H
#define NARROWPHASE_H


namespace gpp
{

class NarrowPhase
{
public:
void scan(const CollisionPairList& pairs, CollisionPairList& colisions);
void scan(const CollisionPairList& pairs, CollisionPairList& colisions, std::shared_mutex& m);
};
}
#endif
