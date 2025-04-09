

#ifndef PROFILEROBJECT_H
#define PROFILEROBJECT_H

namespace gpp{

class ProfilerManager;
class ProfilerObject{
private:
std::string name;
int64 tstart;
int64 tend;
ProfilerManager* handle;

public:

ProfilerObject(const std::string& name, ProfilerManager* gm=nullptr);
ProfilerObject(const ProfilerObject& gm)=delete;
ProfilerObject& operator=(const ProfilerObject& gm)=delete;
~ProfilerObject();

void release();
};
}
#endif
