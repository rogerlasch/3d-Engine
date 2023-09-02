

/**
**/
#ifndef DEFAULTMOTION_H
#define DEFAULTMOTION_H


namespace gpp
{

class DefaultMotion : public iMotion
{
public:
virtual void moveBodyList(std::vector<iRigidBody*>& hbodies, float dt);
virtual void moveBody(iRigidBody* rb, float dt);
};
}
#endif
