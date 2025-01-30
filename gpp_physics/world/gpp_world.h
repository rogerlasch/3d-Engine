

#ifndef GPP_WORLD_H
#define GPP_WORLD_H

#include<vector>
#include<thread>
#include"WorldInfo.h"
#include"EventFrame.h"
#include"ClockFrame.h"
#include"RigidBody.h"

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
WorldInfo* hprops;
ClockFrame* hclock;
std::vector<RigidBody*> bodies;
octree* hgeo;

public:
gpp_world();
gpp_world(const WorldInfo& pw);
gpp_world(const gpp_world& gw)=delete;
gpp_world& operator=(const gpp_world& pw)=delete;
virtual ~gpp_world();

inline uint32 getState()const{return hstate.load();}
std::string toString()const;
bool create(const WorldInfo& hprops);
inline WorldInfo getProperties()const{return *this->hprops;}

bool pushBody(RigidBody* rb);
bool removeBody(uint32 id);
bool removeBody(RigidBody* rb);
RigidBody* getBody(uint32 id)const;
void rayCast(const vector3d& origin, const vector3d& dir, std::vector<RayInfo>& infos);
uint32 pushEventClock(uint32 hframes, EVENTFRAMECALLBACK hcall, void* userData=NULL, void* v1=NULL, void* v2=NULL);
uint32 pushEventClock(EventFrame * ev);
bool removeEventClock(uint32 id);

void update(decimal dt);
};
}
#endif
