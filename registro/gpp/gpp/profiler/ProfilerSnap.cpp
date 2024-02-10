

#include"profiler.h"
#include"ProfilerSnap.h"

using namespace std;

namespace gpp
{
ProfilerSnap::ProfilerSnap(const string& func_name)
{
this->func_name=func_name;
start_time=get_timestamp_ns();
}

ProfilerSnap::~ProfilerSnap()
{
int64 end_time=get_timestamp_ns();
int64 final_time=(end_time-start_time);
profiler_log_snap(func_name, final_time);
}
}
