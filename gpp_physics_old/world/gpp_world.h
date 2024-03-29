

/**
*Aqui � onde ocorrer� toda nossa simula��o.
*Esta classe representar� nosso universo onde poderemos adicionar v�rios corpos, e deixar a f�sica agir sobre eles.
**/
#ifndef GPP_WORLD_H
#define GPP_WORLD_H

#include<bitset>
#include<set>
#include<map>
#include"WorldInfo.h"
#include"WorldCallbacks.h"

namespace gpp
{

class gpp_world
{
public:
std::unique_ptr<WorldInfo> info;
std::unique_ptr<WorldCallbacks> hcon;
std::vector<iRigidBody*> bodies;//Uma lista de todos os corpos existentes no mundo...
std::vector<iRigidBody*> dynamicbodies;
std::vector<iRigidBody*> rbDeletions;
std::unique_ptr<octree> geo;
std::unique_ptr<CollisionCache> hcache;
public:
gpp_world(WorldInfo* winfo=NULL, WorldCallbacks* hcon=NULL);
gpp_world(const gpp_world& w)=delete;
gpp_world& operator=(const gpp_world& w)=delete;
virtual ~gpp_world();
std::string toString()const;
void setGravity(const vector3d& gravity);
vector3d getGravity()const;
void addBody(iRigidBody* rb);
bool removeBody(const gpp_index& id);
iRigidBody* getBody(const gpp_index& id)const;
void update(float dt);//O principal m�todo de atualiza��o do mundo, verifique o cpp para mais detalhes...
private:
void pushBodyDelete(iRigidBody* rb);
void processDeletions();
};
}
#endif
