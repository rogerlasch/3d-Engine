
/**
*Classe para armazenar informações sobre uma colisão...
**/
#ifndef COLLISIONINFO_H
#define COLLISIONINFO_H

namespace gpp
{

class iRigidBody;
class CollisionInfo
{
public:
vector3d point;
vector3d normal;
float depth;
iRigidBody* r1;
iRigidBody* r2;
CollisionInfo();
CollisionInfo(const CollisionInfo& c);
CollisionInfo& operator=(const CollisionInfo& c);
virtual ~CollisionInfo()=default;
void reset();
std::string toString()const;
};
}
#endif
