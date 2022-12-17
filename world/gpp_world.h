

#ifndef GPP_WORLD_H
#define GPP_WORLD_H

#include<bitset>
#include<map>
#ifndef GPP_H
#include"..\\..\\gpp\\gpp.h"
#endif
#include"geometry/geometry.h"
#include"IndexTable.h"
#include"RigidBody.h"
#include"octree.h"
#include"gpp_ground.h"
#include"WordEvent.h"
#include"BroadPhase.h"
#include"NarrowPhase.h"
#include"CollisionSolver.h"

namespace gpp
{

typedef std::function<void(WordEvent*)> WORDEVENTCALLBACK;
class gpp_world : public AABB
{
private:
vector3d gravity;
WORDEVENTCALLBACK hcall;
std::vector<RigidBody*> bodies;
public:
gpp_world();
gpp_world(const gpp_world& w)=delete;
gpp_world& operator=(const gpp_world& w)=delete;
virtual ~gpp_world();
void setWordEventCallBack(WORDEVENTCALLBACK hcall);
WORDEVENTCALLBACK getWordEventCallBack()const;
void setGravity(const vector3d& gravity);
vector3d getGravity()const;
std::vector<RigidBody*>& getBodies();
void addBody(RigidBody* rb);
bool removeBody(const gpp_index& id);
RigidBody* getBody(const gpp_index& id)const;
void update(float dt);
std::string toString()const;
private:
void runcallback(uint32 type, void* v1, void* v2);
};
}
#endif
