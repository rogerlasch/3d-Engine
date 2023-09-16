


#ifndef IFILTER_H
#define IFILTER_H

namespace gpp
{

        class WorldInfo;
class iFilter{
public:
virtual void filter(std::vector<iRigidBody*>& hinput, std::vector<iRigidBody*>& hout, WorldInfo* info, CollisionCache* hcache)=0;
virtual void filter_and_prepare(iRigidBody* rb, std::vector<iRigidBody*>& hbodies, WorldInfo* info, CollisionCache* hcache)=0;
};
}
#endif
