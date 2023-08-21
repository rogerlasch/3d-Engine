
#include"../geometry/geometry.h"
#include"../interfaces/interfaces.h"
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
}
}
