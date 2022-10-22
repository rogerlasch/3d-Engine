


#ifndef RIGIDBODY_H
#define RIGIDBODY_H

namespace gpp
{

enum RIGIDBODYFLAGS
{
RB_STATIC=0,
RB_TRANSPARENT
};

enum RIGIDBODY_TYPES
{
RB_DEFAULT=0,
RB_ZONE,
RB_OBJECT,
RB_NPC
};

class RigidBody
{
protected:
float mass;
public:
gpp_index index;
protected:
vector3d position;
vector3d velocity;
vector3d forse;
std::string name;
std::bitset<16> bflags;
shape2d* body;
public:
RigidBody();
RigidBody(const RigidBody& rb)=delete;
RigidBody& operator=(const RigidBody& rb)=delete;
virtual ~RigidBody();
void setName(const std::string& name);
std::string getName()const;
void setPosition(const vector3d& position);
vector3d getPosition()const;
void setBody(shape2d* body);
shape2d* getBody()const;
void setVelocity(const vector3d& velocity);
vector3d getVelocity()const;
void setForse(const vector3d& forse);
vector3d getForse()const;
void setMass(float mass);
float getMass()const;
void setVnum(uint32 vnum);
uint32 getVnum()const;
void setSubVnum(uint32 vnum);
uint32 getSubVnum()const;
void setIndex(const gpp_index& id);
gpp_index getIndex()const;
virtual void onStep(float dt, vector3d* onchanged=NULL);
std::string toString();
virtual void translate(const vector3d& v);
virtual void rotate(float angle, const vector3d& origin);
void setBodyFlag(const std::initializer_list<uint32>& bflags);
void removeBodyFlag(const std::initializer_list<uint32>& bflags);
bool containsBodyFlag(const std::initializer_list<uint32>& bflags)const;
bool isStatic()const;
bool isTransparent()const;
};
}
#endif
