

#ifndef RIGIDBODY_H
#define RIGIDBODY_H

namespace gpp {

enum RIGIDBODYFLAGS{
BF_DEFAULT=0,
BF_STATIC=(1<<0),//O corpo é estático, não reage a forças externas. Mass==0 sempre terá essa flag.
BF_GRAVITY=(1<<1),//O corpo é afetado pela gravidade...
BF_SENSOR=(1<<2),//O corpo detecta colisão, mas não é corrigido. Exemplo: Um radar aéreo.
BF_REGION=(1<<3),//O corpo representa uma região do mundo. Pode detectar colisão, mas não resolve, pois os limites não são físicos.
BF_SLEEPING=(1<<4)//O corpo está em repouso. Nada de atualização.
};

typedef std::function<void(RigidBody*, RigidBody*, CollisionInfo*)> COLLISIONNOTIFICATIONCALLBACK;

class gpp_world;
class RigidBody {
private:

uint32 id;
uint32 bFlags;
void* userData;

    // Propriedades físicas
    decimal mass;
    decimal inverseMass;
    matrix3x3 inertia;
    matrix3x3 inverseInertia;
    vector3d linearVelocity;
    vector3d angularVelocity;
    vector3d force;
    vector3d torque;

    // Propriedades de estado
    GeometricShape* hbody;

    // Propriedades de colisão
    decimal restitution;
decimal staticFriction;
    decimal dynamicFriction;

    decimal linearDamping;
    decimal angularDamping;

RigidBody* hparent;

gpp_world* hworld;
std::unordered_set<uint64> collisionObjects;
std::unordered_set<RigidBody*> childs;
std::unordered_map<uint32, COLLISIONNOTIFICATIONCALLBACK> CollisionNotifications;

public:
    // Construtor e destrutor
    RigidBody(RigidBody* hparent, uint32 bFlags, decimal mass = 1.0f, GeometricShape* shape = nullptr);
    RigidBody(const RigidBody&) = delete;
    RigidBody& operator=(const RigidBody&) = delete;
    ~RigidBody();

std::string toString()const;

void setId(uint32 id){this->id=id;}
uint32 getId()const{return this->id;}

void setFlags(uint32 bFlags){this->bFlags=bFlags;}
uint32 getFlags()const{return this->bFlags;}

void setWorld(gpp_world* hworld){this->hworld=hworld;}
gpp_world* getWorld()const{return this->hworld;}
void setUserData(void* userData){this->userData=userData;}
void* getUserData()const{return this->userData;}

//Manipulação de flags...

void enableFlag(uint32 flag, bool b);
bool hasFlag(uint32 flags)const;

    // Aplicação de forças e torques
    void applyForce(const vector3d& force);
    void applyForceAtPoint(const vector3d& force, const vector3d& point);
    void applyTorque(const vector3d& torque);
    void clearForces();

    // Getters e Setters
    decimal getMass() const { return mass; }
    void setMass(decimal m);

void setInverseMass(decimal m){this->inverseMass=m;}
decimal getInverseMass()const{return this->inverseMass;}

decimal getStaticFriction()const{return this->staticFriction;}
void setStaticFriction(decimal staticFriction){this->staticFriction=staticFriction;}

decimal getDynamicFriction()const{return this->dynamicFriction;}
void setDynamicFriction(decimal dynamicFriction){this->dynamicFriction=dynamicFriction;}

    const vector3d& getLinearVelocity() const { return linearVelocity; }
    void setLinearVelocity(const vector3d& velocity) { linearVelocity = velocity; }

    const vector3d& getAngularVelocity() const { return angularVelocity; }
    void setAngularVelocity(const vector3d& velocity) { angularVelocity = velocity; }

Transform* getTransform() const { return hbody->getTransform();}

    GeometricShape* getShape() const { return this->hbody;}
    void setShape(GeometricShape* s);

    // Utilidades
    matrix4x4 getWorldTransform() const;
    vector3d getVelocityAtPoint(const vector3d& point) const;

    // Colisão
void pushCollisionId(uint64 colid);
void removeCollisionId(uint64 colId);
bool rayCast(RayInfo* info);
bool rayCast(const vector3d& origin, const vector3d& dir, std::vector<RayInfo>& infos);
virtual void calculateForces(const vector3d& gravity);
virtual     void update(decimal deltaTime);
virtual     void resolveCollision(RigidBody* other, CollisionInfo* info);

void rotate(decimal x, decimal y, decimal z);
void rotate(const quaternion& q);
void translate(const vector3d& translation);
void getAABB(AABB* ab)const;
RigidBody* getParent()const{return this->hparent;}
void setParent(RigidBody* rb){this->hparent=rb;}

void pushBody(RigidBody* rb);
void removeBody(RigidBody* rb);
void removeAllBodies();

void registerCollisionCallback(uint32 type, COLLISIONNOTIFICATIONCALLBACK hcall);
void removeCollisionCallback(uint32 type);
void executeCollisionCallback(uint32 type, RigidBody* rb, CollisionInfo* info);

private:

    // Métodos internos
    void updateInertia();
};
} // namespace gpp
#endif // RIGIDBODY_H
