


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

class RigidBody : public AABB
{
public:
gpp_index index;
protected:
uint32 btype;
uint32 userdata;
float mass;
vector3d velocity;
vector3d forse;
std::string name;
SafeFlags<uint16> bflags;
public:
RigidBody(uint32 btype=0);
RigidBody(const RigidBody& rb)=delete;
RigidBody& operator=(const RigidBody& rb)=delete;
virtual ~RigidBody();
uint32 getBType()const;
void setVnum(uint32 vnum);
uint32 getVnum()const;
void setSubVnum(uint32 vnum);
uint32 getSubVnum()const;
void setIndex(const gpp_index& id);
gpp_index getIndex()const;
void setName(const std::string& name);
std::string getName()const;
void setUserData(uint32 udata);
uint32 getUserData()const;
void setVelocity(const vector3d& velocity);
vector3d getVelocity()const;
void setForse(const vector3d& forse);
vector3d getForse()const;
void setMass(float mass);
float getMass()const;
virtual vector3d nextStep(float dt);
std::string toString();
void setBodyFlag(uint16 bflags);
void removeBodyFlag(uint16 bflags);
bool containsBodyFlag(uint16 bflags);
bool isStatic()const;
bool isTransparent()const;
friend class BroadPhase;
};
typedef std::vector<RigidBody*> RigidBodyList;
}
#endif
