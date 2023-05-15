

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

class RigidBody : public AABB
{
public:
gpp_index index;
//protected:
uint32 btype;
uint32 userdata;
float mass;//Maça do corpo
float inverseMass;
    float linearDamping;
    float angularDamping;
float restitution;
vector3d position;//Posição do corpo...
vector3d linearMomentum;
vector3d angularMomentum;
vector3d linearVelocity;
vector3d angularVelocity;
vector3d linearAcceleration;
vector3d angularAcceleration;
    vector3d force;
    vector3d torque;
matrix3x3 inertiaTensor;
matrix3x3 inverseInertiaTensor;
quaternion orientation;
std::string name;
mutable std::atomic<uint16> bflags;
public:
RigidBody(uint32 btype=0);
RigidBody(const RigidBody& rb)=delete;
RigidBody& operator=(const RigidBody& rb)=delete;
virtual ~RigidBody();
uint32 getBType()const;//Recupera o tipo do corpo rígido...
void setVnum(uint32 vnum);//Define o id principal do corpo
uint32 getVnum()const;//retorna o Id principal do corpo
void setSubVnum(uint32 vnum);//defineo id secundário do corpo...
uint32 getSubVnum()const;//Retorna o id secundário do corpo
void setIndex(const gpp_index& id);
gpp_index getIndex()const;
void setName(const std::string& name);
std::string getName()const;
void setUserData(uint32 udata);
uint32 getUserData()const;
std::string toString();// Imprime todas propriedades do corpo rígido...
void setBodyFlag(uint16 bf);
void removeBodyFlag(uint16 bf);
bool containsBodyFlag(uint16 bf)const;
bool isStatic()const;
bool isTransparent()const;
//Geters e seters...
    // Getters e setters
    float getMass()const;
    void setMass(float m);
float getInverseMass()const;
void setInverseMass(float inv);
    float getLinearDamping()const;
    void setLinearDamping(float ld);
    float getAngularDamping()const;
    void setAngularDamping(float ad);
float getRestitution()const;
void setRestitution(float restitution);
    vector3d getPosition()const;
    void setPosition(const vector3d &p);
    vector3d getLinearMomentum()const;
    void setLinearMomentum(const vector3d &lm);
    vector3d getAngularMomentum()const;
    void setAngularMomentum(const vector3d &am);
    vector3d getLinearVelocity()const;
    void setLinearVelocity(const vector3d &lv);
    vector3d getAngularVelocity()const;
    void setAngularVelocity(const vector3d &av);
    vector3d getLinearAcceleration()const;
    void setLinearAcceleration(const vector3d &la);
    vector3d getAngularAcceleration()const;
    void setAngularAcceleration(const vector3d &aa);
    vector3d getForce()const;
    void setForce(const vector3d &f);
    vector3d getTorque()const;
    void setTorque(const vector3d &t);
    matrix3x3 getInertiaTensor()const;
    void setInertiaTensor(const matrix3x3 &it);
    matrix3x3 getInverseInertiaTensor()const;
void setInverseInertiaTensor(const matrix3x3& mt);
quaternion getOrientation()const;
void setOrientation(const quaternion& q);
friend class BroadPhase;
friend class IntegratorVerlet;
friend class CollisionSolver;
};
typedef std::vector<RigidBody*> RigidBodyList;

void initRigidBody(RigidBody* rb, GeometricShape* sh, float mass=1.0f);
}
#endif
