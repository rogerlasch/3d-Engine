
#ifndef PROFILERMANAGER_H
#define PROFILERMANAGER_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include "ProfilerData.h"
#include "ProfilerObject.h"

namespace gpp {

class ProfilerObject;
class ProfilerManager{
private:
    std::string filename;
    uint32 resolution;
    std::unordered_map<std::string, ProfilerData> hdata;
    std::unordered_set<ProfilerObject*> activeProfilers;

public:
    ProfilerManager(uint32 resolution=PF_NANO, const std::string& filename="default gpp_profiler.txt");
    ProfilerManager(const ProfilerManager& pm) = delete;
    ProfilerManager& operator=(const ProfilerManager& pm) = delete;
    ~ProfilerManager();

inline void setResolution(uint32 resolution){this->resolution=resolution;}
inline uint32 getResolution()const{return resolution;}

std::string dump()const;
    void dumpToFile();

private:
void registreProfile(ProfilerObject* obj, const std::string& name, int64 ts);
void pushObject(ProfilerObject* obj);
void removeObject(ProfilerObject* obj);

friend class ProfilerObject;
};
} // namespace gpp
#endif // PROFILERMANAGER_H
