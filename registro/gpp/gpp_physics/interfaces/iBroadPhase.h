


#ifndef IBROADPHASE_H
#define IBROADPHASE_H

namespace gpp
{
class iBroadPhase
{
public:
iBroadPhase()=default;
virtual ~iBroadPhase()=default;
virtual void scan(std::vector<iRigidBody*>& bodies, CollisionCache* cache)=0;
};
}
#endif
