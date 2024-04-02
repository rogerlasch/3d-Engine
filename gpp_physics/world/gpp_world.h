

#ifndef GPP_WORLD_H
#define GPP_WORLD_H

#include<vector>
#include<thread>
#include"WorldProperties.h"
#include"BodyHandler.h"
#include"RigidBody.h"
#include"BodyHelper.h"

namespace gpp{


enum GPP_WORLD_STATES{
GWF_DEFAULT=0,
GWF_ACTIVE,
GWF_RUNNING,
GWF_STOPPING
};

class octree;
class gpp_world{
private:
std::atomic<uint32> hstate;
WorldProperties* hprops;
std::vector<RigidBody*> bodies;
octree* hgeo;
std::unordered_map<uint64, CollisionInfo*> collisions;
std::thread handle;

public:
gpp_world();
gpp_world(const WorldProperties& pw);
gpp_world(const gpp_world& gw)=delete;
gpp_world& operator=(const gpp_world& pw)=delete;
virtual ~gpp_world();

inline uint32 getState()const{return hstate.load();}
std::string toString()const;
bool create(const WorldProperties& hprops);
inline WorldProperties getProperties()const{return *this->hprops;}

bool addBody(RigidBody* rb);
bool removeBody(uint32 id);
bool removeBody(RigidBody* rb);
RigidBody* getBody(uint32 id)const;
inline CollisionInfo* getCollision(uint64 id)const{ auto it=collisions.find(id); return ((it==collisions.end()) ? NULL : it->second);}
void rayCast(const vector3d& origin, const vector3d& dir, std::vector<RayInfo>& infos);

bool start();
void stop();

void update(decimal dt);

private:
void gameLoop();
void internalUpdate(decimal dt);
void selectAndPrepareBodies(std::vector<RigidBody*>& bodies);
void integrateBodies(std::vector<RigidBody*>& bodies);
void broadPhase(std::vector<CollisionInfo*>& collisions);
void narrowPhase(std::vector<CollisionInfo*>& hcollisions);
void solveCollisions();
void solvePair(CollisionInfo* info);
void dispatchCollisions();
};
}
#endif
