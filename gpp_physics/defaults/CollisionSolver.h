
/**
*Classe que implementa um solucionador de colis�es.
*Na f�sica, quando 2 corpos se chocam, uma s�rie de coisas precisa acontecer.
*Calcular o impulso resultante para cada corpo � uma dessas coisas. Ou seja, cada vez que uma bola atinge o ch�o, ela deve quicar cada vez mais baixo at� por sim ficar em repouso no ch�o.
*Esta classe precisa do ponto de contato da colis�o entre os corpos, o vetor normal da dire��o da colis�o, e tamb�m a profundidade de penetra��o, para poder afastar os corpos de forma que eles n�o colidam mais.
**/
#ifndef COLLISIONSOLVER_H
#define COLLISIONSOLVER_H


namespace gpp
{

class CollisionSolver : public iCollisionSolver
{
public:
void solve(std::vector<CollisionInfo>& collisions);
void solvePair(CollisionInfo& info);
};
}
#endif
