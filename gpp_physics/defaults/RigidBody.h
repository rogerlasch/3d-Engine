

/**
*Esta classe representa um corpo em nossa simulação.
*Um corpo tem forma, velocidades, forças e maça.
*Caso o corpo não tenha maça, ele será considerado um corpo estático, que não se move. Ele será necessário para modelar o chão, terrenos, e paredes.
**/
#ifndef RIGIDBODY_H
#define RIGIDBODY_H

namespace gpp
{

enum RIGIDBODYFLAGS
{
RB_STATIC=(1<<0),
RB_TRANSPARENT=(1<<1)
};

enum RIGIDBODY_TYPES
{
RB_DEFAULT=0,
RB_ZONE,
RB_OBJECT,
RB_PC,
RB_NPC
};

class RigidBody : public iRigidBody
{
public:
RigidBody(uint32 btype=0);
RigidBody(const RigidBody& rb)=delete;
RigidBody& operator=(const RigidBody& rb)=delete;
virtual ~RigidBody();
virtual std::string toString()const;
};
}
#endif
