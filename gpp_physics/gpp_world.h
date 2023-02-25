

/**
*Aqui � onde ocorrer� toda nossa simula��o.
*Esta classe representar� nosso universo onde poderemos adicionar v�rios corpos, e deixar a f�sica agir sobre eles.
**/
#ifndef GPP_WORLD_H
#define GPP_WORLD_H

#include<bitset>
#include<map>

namespace gpp
{

class gpp_world : public AABB
{
private:
vector3d gravity;//A gravidade do nosso mundo...
std::vector<RigidBody*> bodies;//Uma lista de todos os corpos existentes no mundo...
public:
gpp_world();
gpp_world(const gpp_world& w)=delete;
gpp_world& operator=(const gpp_world& w)=delete;
virtual ~gpp_world();
void setGravity(const vector3d& gravity);
vector3d getGravity()const;
std::vector<RigidBody*>& getBodies();
void addBody(RigidBody* rb);
bool removeBody(const gpp_index& id);
RigidBody* getBody(const gpp_index& id)const;
void update(float dt);//O principal m�todo de atualiza��o do mundo, verifique o cpp para mais detalhes...
std::string toString()const;
private:
};
}
#endif
