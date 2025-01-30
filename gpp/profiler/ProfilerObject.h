

#ifndef PROFILEROBJECT_H
#define PROFILEROBJECT_H

namespace gpp{

class ProfilerManager;
class ProfilerObject{
private:
std::string name;
int64 tstart;
int64 tend;
bool active;
ProfilerManager* handle;

public:

ProfilerObject(const std::string& name, ProfilerManager* gm);
ProfilerObject(const ProfilerObject& gm)=delete;
ProfilerObject& operator=(const ProfilerObject& gm)=delete;
~ProfilerObject();

void start();
void stop();
void release();
};
}
#endif
