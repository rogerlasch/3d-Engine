
/**
*Classe para armazenar informações sobre uma colisão...
**/

#ifndef COLLISIONINFO_H
#define COLLISIONINFO_H

namespace gpp
{

#define NUMFRAMES 3

class RigidBody;
class CollisionInfo
{
public:
uint64 id;
bool notify;
vector3d point;
vector3d normal;
float depth;
RigidBody* r1;
RigidBody* r2;
uint32 counter;
CollisionInfo();
CollisionInfo(const CollisionInfo& c);
CollisionInfo& operator=(const CollisionInfo& c);
virtual ~CollisionInfo()=default;
bool operator<(const CollisionInfo& info)const;
uint64 getId()const;
void reset();
std::string toString()const;
};
}
#endif
