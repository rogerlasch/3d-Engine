


#ifndef IBROADPHASE_H
#define IBROADPHASE_H

namespace gpp
{
class iBroadPhase
{
public:
iBroadPhase()=default;
virtual ~iBroadPhase()=default;
void scan(std::vector<iRigidBody*>& bodies, std::vector<CollisionInfo>& collisions);
};
}
#endif
