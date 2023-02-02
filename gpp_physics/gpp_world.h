

#ifndef GPP_WORLD_H
#define GPP_WORLD_H

#include<bitset>
#include<map>

namespace gpp
{

class gpp_world : public AABB
{
private:
vector3d gravity;
std::vector<RigidBody*> bodies;
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
void update(float dt);
std::string toString()const;
private:
};
}
#endif
