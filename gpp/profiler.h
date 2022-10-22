

#ifndef PROFILER_H
#define PROFILER_H

namespace gpp
{

void profiler_start();
void profiler_reset();
void profiler_log(const std::string& func_name, int64 ms);
void profiler_dump(const std::string& filename);

class ProfilerSnap
{
private:
std::string func_name;
int64 start;
public:
ProfilerSnap(const std::string& func_name);
~ProfilerSnap();
};

#define profiler_snap() ProfilerSnap SNP(__FUNCTION__)
}
#endif
