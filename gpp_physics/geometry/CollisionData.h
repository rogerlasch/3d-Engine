
/**
*Classe para armazenar informações sobre uma colisão...
**/
#ifndef COLLISIONDATA_H
#define COLLISIONDATA_H

namespace gpp
{

class CollisionData
{
public:
vector3d point;
vector3d normal;
float depth;
CollisionData();
CollisionData(const CollisionData& c);
CollisionData& operator=(const CollisionData& c);
virtual ~CollisionData()=default;
void reset();
std::string toString()const;
};

std::ostream& operator<<(std::ostream& os, const CollisionData& cd);
}
#endif
