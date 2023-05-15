

#ifndef IMOTION_H
#define IMOTION_H

namespace gpp
{
class iMotion
{
public:
iMotion()=default;
virtual ~iMotion()=default;
virtual void moveBody(iRigidBody* rb)=0;
};
}
#endif
