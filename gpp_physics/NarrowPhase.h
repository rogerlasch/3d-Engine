

/**
*Nesta classe � onde verificamos se 2 corpos est�o de fato colidindo.
*Aqui � onde faremos os testes computacionalmente caros entre geometrias espec�ficas como esfera x caixa, caixa x caixa, e etc...
*Caso uma colis�o de fato esteja sendo detectada, ela deve adicionar o par de corpos bem como informa��es da colis�o em uma lista para o solucionador de colis�es resolver elas.
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
