
/**
*Classe para armazenar informações sobre uma colisão...
**/

#ifndef COLLISIONINFO_H
#define COLLISIONINFO_H

namespace gpp
{

enum COLLISION_TYPES{
COL_NONE=0,
COL_BEGIN,
COL_CONTINUOUS,
COL_END
};

class GeometricShape;
class CollisionInfo
{
public:
uint64 id;
uint32 type;
uint32 frames;
vector3d point;
vector3d lastPoint;
vector3d normal;
vector3d lastNormal;
decimal depth;
decimal lastDepth;
GeometricShape* r1;
GeometricShape* r2;
CollisionInfo();
CollisionInfo(const CollisionInfo& c);
CollisionInfo& operator=(const CollisionInfo& c);
virtual ~CollisionInfo()=default;
bool operator<(const CollisionInfo& info)const;
uint64 getId()const;
void reset();
std::string toString()const;
void backup();
void update(CollisionInfo* info);
};

uint64 getCollisionId(uint64 id1, uint64 id2);
}
#endif
