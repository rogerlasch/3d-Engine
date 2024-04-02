


#ifndef GPP_WORLD_H
#define GPP_WORLD_H

#include<set>
#include"math/math.h"
#include"RigidBody.h"
#include"Sphere3d.h"
#include"Box3d.h"
#include"Capsule3d.h"
#include"CollisionInfo.h"
#include"collision3d.h"
#include"octree/octree.h"

namespace gpp{

enum GPP_WORLD_STATS{
GW_DEFAULT=0,
GW_STARTING,
GW_RUNNING,
GW_CLOSING,
GW_FINISHED
};

enum GPP_WORLD_FLAGS{
GWF_GRAVITY=(1<<0),
GWF_DAMPING=(1<<1),
GWF_BROADPHASE=(1<<2)
};

#define GPP_WORLD_DEFAULT_SIZE 10000000.0f

class gpp_world{
private:

float linearDamping;
float angularDamping;
float currentTime;
float timestep;

uint32 gsortaxis;
vector3d min;
vector3d max;
vector3d gravity;


std::thread handle;
StateManager* hstate;

std::map<uint64, CollisionInfo> collisions;
std::vector<RigidBody*> dynamicBodies;
std::vector<RigidBody*> staticBodies;
octree* geography;
public:

gpp_world(const vector3d& min={0,0,0}, const vector3d& alfSize={GPP_WORLD_DEFAULT_SIZE, GPP_WORLD_DEFAULT_SIZE, GPP_WORLD_DEFAULT_SIZE}, uint32 flags=GWF_GRAVITY|GWF_DAMPING|GWF_BROADPHASE);
gpp_world(const gpp_world& gw)=delete;
gpp_world& operator=(const gpp_world& gw)=delete;
virtual ~gpp_world();

virtual std::string toString()const;

inline void getMinMax(vector3d& wmin, vector3d& wmax){wmin=min; wmax=max;}
inline float getCurrentTime()const{return currentTime;}
inline void setTimeStep(float timestep){this->timestep=timestep;}
inline float getTimeStep()const{return this->timestep;}
inline void setLinearDamping(float d){this->linearDamping=d;}
inline float getLinearDamping()const{return this->linearDamping;}
inline void setAngularDamping(float d){this->angularDamping=d;}
inline float getAngularDamping()const{return this->angularDamping;}
inline void setGravity(const vector3d& v){this->gravity=v;}
inline vector3d getGravity()const{return this->gravity;}

inline bool isStarted()const{return hstate->getState()==GW_RUNNING;}
inline bool isEnabled(uint32 f) { return hstate->flag_contains(f); }
virtual bool enable(uint32 f, bool en);

bool start();
void stop();

void addObject(RigidBody* rb);
void removeObject(RigidBody* rb);
RigidBody* getObject(uint32 id)const;
void removeAllObjects();


void internalUpdate(float dt);
void prepareBodies(float dt);
void integrate(float dt);
void internalBroadPhase(float dt, std::vector<CollisionInfo>& hcollisions);
void internalNarrowPhase(float dt, std::vector<CollisionInfo>& hcollisions);
void basicDetectCollision(float dt);
void solveCollision(RigidBody* r1, RigidBody* r2, CollisionInfo* info);
void updateCollisions(float dt);
private:

void onLoop();
};
}
#endif
