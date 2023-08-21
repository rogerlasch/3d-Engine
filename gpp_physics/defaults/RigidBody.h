

/**
*Esta classe representa um corpo em nossa simula��o.
*Um corpo tem forma, velocidades, for�as e ma�a.
*Caso o corpo n�o tenha ma�a, ele ser� considerado um corpo est�tico, que n�o se move. Ele ser� necess�rio para modelar o ch�o, terrenos, e paredes.
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
