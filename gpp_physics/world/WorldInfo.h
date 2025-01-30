

#ifndef WORLDINFO_H
#define WORLDINFO_H

namespace gpp{
class WorldInfo{
public:
vector3d gravity;
vector3d center;
decimal alf;
WorldInfo();
WorldInfo(const WorldInfo& wf);
WorldInfo& operator=(const WorldInfo& wf);

std::string toString()const;
void cleanup();
void toDefault();
};
}
#endif
