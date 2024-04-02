

#ifndef RAYINFO_H
#define RAYINFO_H

namespace gpp{

class RayInfo{
public:
decimal eDist;
decimal oDist;
vector3d ePoint;
vector3d oPoint;
uint64 targetId;
RayInfo();
RayInfo(const RayInfo& info);
RayInfo& operator=(const RayInfo& info);
std::string toString()const;
void cleanup();
};
}
#endif
