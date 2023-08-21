

/**
*Aqui é onde ocorrerá toda nossa simulação.
*Esta classe representará nosso universo onde poderemos adicionar vários corpos, e deixar a física agir sobre eles.
**/
#ifndef GPP_WORLD_H
#define GPP_WORLD_H

#include<bitset>
#include<set>
#include<map>

namespace gpp
{

class gpp_world : public AABB
{
private:
vector3d gravity;//A gravidade do nosso mundo...
std::vector<iRigidBody*> bodies;//Uma lista de todos os corpos existentes no mundo...
std::vector<iRigidBody*> dynamicbodies;
std::vector<iRigidBody*> rbDeletions;
std::shared_ptr<octree> geo;
public:
gpp_world();
gpp_world(const gpp_world& w)=delete;
gpp_world& operator=(const gpp_world& w)=delete;
virtual ~gpp_world();
std::string toString()const;
void setGravity(const vector3d& gravity);
vector3d getGravity()const;
void addBody(iRigidBody* rb);
bool removeBody(const gpp_index& id);
iRigidBody* getBody(const gpp_index& id)const;
void update(float dt);//O principal método de atualização do mundo, verifique o cpp para mais detalhes...
private:
void pushBodyDelete(iRigidBody* rb);
void processDeletions();
};
}
#endif
