

#include"profiler.h"
#include"ProfilerSnap.h"

using namespace std;

namespace gpp
{
ProfilerSnap::ProfilerSnap(const string& func_name)
{
this->func_name=func_name;
this->start_time = chrono::high_resolution_clock::now();
}

ProfilerSnap::~ProfilerSnap()
{
chrono::high_resolution_clock::time_point end_time=chrono::high_resolution_clock::now();
auto final_time=chrono::duration_cast<chrono::nanoseconds>(end_time-start_time);
profiler_log_snap(func_name, final_time.count());
}
}
