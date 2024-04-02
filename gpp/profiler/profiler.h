

#ifndef GPP_PROFILER_H
#define GPP_PROFILER_H

#include<string>
#ifndef GPP_H
#include"../types.h"
#include"../time.h"
#include"../gpp_value.h"
#endif
#include"ProfilerSnap.h"

namespace gpp
{

enum PROFILER_DUMP_TIME{
PFD_NANOSECONDS=0,
PFD_MICROSECONDS,
PFD_MILLISECONDS
};

void profiler_start();
void profiler_dump(const std::string& filename, uint32 tType=PFD_MILLISECONDS);
void profiler_log_snap(const std::string& func_name, int64 execution_time_ns);
decimal profiler_format_time(int64 ns, uint32 tType);
std::string profiler_get_time_desk(uint32 tType);

#if defined(_DEBUG)||defined(_GPP_USE_DEBUG)
#define profiler_snap() ProfilerSnap PROFILER_SNAP_INFO_DATA(__FUNCTION__)
#else
#define profiler_snap()
#endif
}
#endif
