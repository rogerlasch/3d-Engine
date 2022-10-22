

#ifndef GPP_WORLD_H
#define GPP_WORLD_H

#include<bitset>
#ifndef GPP_H
#include"..\\..\\gpp\\gpp.h"
#endif
#include"gpp_index.h"
#include"vector3d.h"
#include"IndexTable.h"
#include"geometry.h"
#include"world_utils.h"
#include"gpp_floor.h"
#include"gpp_zone.h"
#include"zone_utils.h"

namespace gpp
{

enum WORLD_WHERE_POINT
{
WHERE_UNKNOWN=0,
WHERE_AIR,
WHERE_GROUND,
WHERE_UNDERGROUND
};

class gpp_world
{
private:
float ground_level;
vector3d gravity;
vector3d lower_bound;
vector3d higher_bound;
std::vector<RigidBody*> bodies;
gpp_zone* root;
public:
gpp_world();
gpp_world(const gpp_world& w)=delete;
gpp_world& operator=(const gpp_world& w)=delete;
virtual ~gpp_world();
void setGroundLevel(float lv);
float getGroundLevel()const;
void setGravity(const vector3d& gravity);
vector3d getGravity()const;
void setLowerBound(const vector3d& lowerbound);
vector3d getLowerBound()const;
void setHigherBound(const vector3d& higherbound);
vector3d getHigherBound()const;
void setRoot(gpp_zone* root);
gpp_zone* getRoot()const;
uint32 where_is_point(const vector3d& v)const;
bool point_in_world(const vector3d& v)const;
bool addBody(uint32 vnum, uint32 subvnum);
void add_bodies(uint32 qtd, uint32 subqtd=1);
std::vector<RigidBody*>& getBodies();
void update(float dt);
std::string toString()const;
};
}
#endif
