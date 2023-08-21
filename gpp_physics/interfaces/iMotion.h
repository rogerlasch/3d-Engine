

#ifndef IMOTION_H
#define IMOTION_H

namespace gpp
{
class iMotion
{
public:
iMotion()=default;
virtual ~iMotion()=default;
virtual void moveBodyList(std::vector<iRigidBody*>& hbodies, float dt)=0;
virtual void moveBody(iRigidBody* rb, float dt)=0;
};
}
#endif
