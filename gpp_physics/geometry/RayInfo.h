


#ifndef RAYINFO_H
#define RAYINFO_H

namespace gpp{

class RayInfo{
public:

uint64 id;
void* target;

vector3d origin;
vector3d dir;

decimal enterDist;
decimal outDist;
vector3d enterPoint;
vector3d outPoint;
bool colliding;

RayInfo();
RayInfo(const vector3d& origin, const vector3d& dir);
RayInfo(const RayInfo& rinfo);
RayInfo& operator=(const RayInfo& rinfo);

std::string toString()const;
void clear();

};
}
#endif
