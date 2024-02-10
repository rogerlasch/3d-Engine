

#ifndef GPP_PROFILERSNAP_H
#define GPP_PROFILERSNAP_H

namespace gpp
{

class ProfilerSnap
{
private:
int64 start_time;
std::string func_name;
public:
ProfilerSnap(const std::string& func_name);
ProfilerSnap(const ProfilerSnap& ps)=delete;
ProfilerSnap& operator=(const ProfilerSnap& ps)=delete;
virtual ~ProfilerSnap();
};
}
#endif
