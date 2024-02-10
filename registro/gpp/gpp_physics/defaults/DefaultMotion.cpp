
#include"../gpp_physics.h"
#include"DefaultMotion.h"

using namespace std;

namespace gpp
{
void DefaultMotion::moveBodyList(std::vector<iRigidBody*>& hbodies, float dt)
{
for(auto& it : hbodies)
{
moveBody(it, dt);
}
}

void DefaultMotion::moveBody(iRigidBody* rb, float dt)
{
vector3d acceleration=rb->forces/rb->mass;
rb->velocity=rb->velocity+acceleration*dt;
vector3d pos=(rb->velocity*dt);
rb->translate(pos);
rb->forces={0.0f,0.0f,0.0f};
}
}
